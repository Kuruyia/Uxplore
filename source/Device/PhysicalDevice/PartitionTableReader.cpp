/*
    Uxplore
    Copyright (C) 2019-2021, Kuruyia

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <cstring>
#include <whb/log.h>
#include <zlib.h>

#include "../../Utils.h"
#include "PartitionTableReader.h"

PartitionTableReader::PartitionTableReader(const DiscInterface *discInterface)
        : m_mbr()
        , m_gptPresent(false)
        , m_gptHeader()
        , m_ready(false)
{
    if (readMbrFromDisc(discInterface)) {
        /* We have successfully read the MBR from the disc, let's go deeper:
         * We look for a protective MBR, and if it isn't one, we look for any extended partition
         */

        if (!detectProtectiveMbr(discInterface))
            detectExtendedPartitions(discInterface);

        // Everything's fine, we set the ready flag
        m_ready = true;
    }
}

const PartitionTableReader::MASTER_BOOT_RECORD &PartitionTableReader::getMbr() const
{
    return m_mbr;
}

const std::vector<PartitionTableReader::MBR_PARTITION> &PartitionTableReader::getMbrPartitions() const
{
    return m_mbrPartitions;
}

const std::vector<PartitionTableReader::EBR_PARTITION> &PartitionTableReader::getEbrPartitions() const
{
    return m_ebrPartitions;
}

bool PartitionTableReader::hasGpt()
{
    return m_gptPresent;
}

const PartitionTableReader::GPT_HEADER &PartitionTableReader::getGptHeader() const
{
    return m_gptHeader;
}

const std::vector<PartitionTableReader::GPT_PARTITION_ENTRY> &PartitionTableReader::getGptPartitions() const
{
    return m_gptPartitionEntries;
}

bool PartitionTableReader::isReady() const
{
    return m_ready;
}

bool PartitionTableReader::readMbrFromDisc(const DiscInterface *discInterface)
{
    if (!discInterface->readSectors(0, 1, &m_mbr)) {
        WHBLogPrintf("PartitionTableReader: Error while reading first sector");
        return false;
    }

    WHBLogPrintf("PartitionTableReader: Succesfully read first sector");
    if (m_mbr.signature != 0x55AA && m_mbr.signature != 0x55AB) {
        WHBLogPrintf("PartitionTableReader: Invalid MBR signature found!");
        return false;
    }

    // Extract partitions from MBR
    for (const MBR_PARTITION partition : m_mbr.partitions) {
        // Ensure valid partition
        if (partition.partitionType != 0x00) {
            m_mbrPartitions.push_back(partition);
        }
    }

    // We want little endian
    for (MBR_PARTITION &partition : m_mbrPartitions) {
        correctMbrPartitionEndianness(partition);
    }

    return true;
}

bool PartitionTableReader::detectProtectiveMbr(const DiscInterface *discInterface)
{
    // Check if it's a protective MBR
    if (m_mbrPartitions[0].partitionType == 0xEE) {
        WHBLogPrintf("PartitionTableReader: Protective MBR found!");

        // TODO: Read from the alternative header if the primary header is not valid
        m_gptPresent = readGptFromDisc(discInterface, 1);
    }

    return m_gptPresent;
}

bool PartitionTableReader::readGptFromDisc(const DiscInterface *discInterface, const uint32_t headerSector)
{
    // Read the sector where we expect the header
    if (!discInterface->readSectors(headerSector, 1, &m_gptHeader)) {
        WHBLogPrint("PartitionTableReader: Error while reading GPT header sector");
        return false;
    }

    // Check if the signature is the one we expect
    if (strncmp(m_gptHeader.signature, "EFI PART", 0x08) != 0) {
        WHBLogPrint("PartitionTableReader: GPT header signature is invalid");
        return false;
    }

    // Fix endianness
    GPT_HEADER originalHeader = m_gptHeader;
    correctGptHeaderEndianness(m_gptHeader);

    // Check header's CRC32 (using the untouched header)
    originalHeader.headerCRC32 = 0;
    const uLong headerCrc = crc32(0, reinterpret_cast<const Bytef *>(&originalHeader), m_gptHeader.headerSize);
    if (m_gptHeader.headerCRC32 != headerCrc) {
        WHBLogPrint("PartitionTableReader: GPT header CRC32 does NOT match");
        return false;
    }

    // We read all the GPT partition entry array (assuming sector size of 512 bytes)
    const unsigned sectorsToParse = (m_gptHeader.partitionCount * m_gptHeader.partitionEntrySize) / 512;
    auto *entryArray = new GPT_PARTITION_ENTRY[m_gptHeader.partitionCount];
    if (!discInterface->readSectors(m_gptHeader.partitionEntryLBA, sectorsToParse, entryArray)) {
        WHBLogPrint("PartitionTableReader: Error while reading GPT partition array sectors");

        delete[] entryArray;
        return false;
    }

    // Check all the partitions of the array, and compute the array CRC32
    uLong entryArrayCrc = 0;
    for (unsigned i = 0; i < m_gptHeader.partitionCount; ++i) {
        entryArrayCrc = crc32(entryArrayCrc, reinterpret_cast<const Bytef *>(&entryArray[i]),
                              m_gptHeader.partitionEntrySize);

        // Add the partition if not unused
        if (entryArray[i].partitionTypeGUID.lowPart != 0 || entryArray[i].partitionTypeGUID.highPart != 0) {
            // Correct the partition endianness and add the partition
            correctGptPartitionEndianness(entryArray[i]);
            m_gptPartitionEntries.push_back(entryArray[i]);
        }
    }

    delete[] entryArray;

    if (m_gptHeader.partitionEntryCRC32 != entryArrayCrc) {
        WHBLogPrint("PartitionTableReader: GPT partition array CRC32 does NOT match");
        return false;
    }

    WHBLogPrint("PartitionTableReader: GPT header verifications passed!");
    return true;
}

void PartitionTableReader::detectExtendedPartitions(const DiscInterface *discInterface)
{
    // Check every partition entry in the MBR for an extended partition
    for (const MBR_PARTITION &partition : m_mbrPartitions) {
        discoverExtendedPartition(discInterface, partition, m_ebrPartitions);
    }
}

bool PartitionTableReader::isExtendedPartitionType(const uint8_t partitionType)
{
    return (partitionType == 0x05 || partitionType == 0x0F || partitionType == 0x85);
}

void PartitionTableReader::discoverExtendedPartition(const DiscInterface *discInterface,
                                                     const MBR_PARTITION &extendedPartitionEntry,
                                                     std::vector<EBR_PARTITION> &logicalPartitions)
{
    // Check that the entry partition type is extended partition
    if (!isExtendedPartitionType(extendedPartitionEntry.partitionType))
        return;

    MBR_PARTITION nextEBR;
    uint32_t sectorToCheck = extendedPartitionEntry.startingLBA;

    do {
        MASTER_BOOT_RECORD ebr;

        // Sanity checks
        if (!discInterface->readSectors(sectorToCheck, 1, &ebr))
            break;

        if (ebr.signature != 0x55AA && ebr.signature != 0x55AB)
            break;

        // We need to keep track of this because an EBR entry's starting LBA is relative to the EBR starting LBA
        EBR_PARTITION currentLogicalPartition;
        currentLogicalPartition.ebrLBA = sectorToCheck;

        // Store first two entries
        if (ebr.partitions[0].partitionType != 0x00) {
            correctMbrPartitionEndianness(ebr.partitions[0]);
            currentLogicalPartition.partition = ebr.partitions[0];
            logicalPartitions.push_back(currentLogicalPartition);
        }

        correctMbrPartitionEndianness(ebr.partitions[1]);
        nextEBR = ebr.partitions[1];
        sectorToCheck = nextEBR.startingLBA + extendedPartitionEntry.startingLBA;
    } while (nextEBR.startingLBA != 0);
}

void PartitionTableReader::correctMbrPartitionEndianness(PartitionTableReader::MBR_PARTITION &partition)
{
    partition.startingLBA = Utils::swapEndian32(partition.startingLBA);
    partition.totalBlocks = Utils::swapEndian32(partition.totalBlocks);
}

void PartitionTableReader::correctGptHeaderEndianness(PartitionTableReader::GPT_HEADER &header)
{
    header.revision = Utils::swapEndian32(header.revision);
    header.headerSize = Utils::swapEndian32(header.headerSize);
    header.headerCRC32 = Utils::swapEndian32(header.headerCRC32);
    header.myLBA = Utils::swapEndian64(header.myLBA);
    header.alternateLBA = Utils::swapEndian64(header.alternateLBA);
    header.firstUsableLBA = Utils::swapEndian64(header.firstUsableLBA);
    header.lastUsableLBA = Utils::swapEndian64(header.lastUsableLBA);
    header.partitionEntryLBA = Utils::swapEndian64(header.partitionEntryLBA);
    header.partitionCount = Utils::swapEndian32(header.partitionCount);
    header.partitionEntrySize = Utils::swapEndian32(header.partitionEntrySize);
    header.partitionEntryCRC32 = Utils::swapEndian32(header.partitionEntryCRC32);
}

void PartitionTableReader::correctGptPartitionEndianness(PartitionTableReader::GPT_PARTITION_ENTRY &partition)
{
    partition.startingLBA = Utils::swapEndian64(partition.startingLBA);
    partition.endingLBA = Utils::swapEndian64(partition.endingLBA);
    partition.attributes = Utils::swapEndian64(partition.attributes);
}
