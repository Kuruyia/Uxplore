/*
    Uxplore
    Copyright (C) 2019-2019, Kuruyia

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

#ifndef UFILE_CPP_PARTITIONTABLEREADER_H
#define UFILE_CPP_PARTITIONTABLEREADER_H

#include <string>
#include <vector>
#include <DiscInterface/DiscInterface.h>
#include <memory>

class PartitionTableReader {
public:
    typedef struct {
        uint8_t status;
        uint8_t startingHead;
        uint8_t startingSector;
        uint8_t startingCylinder;
        uint8_t partitionType;
        uint8_t endingHead;
        uint8_t endingSector;
        uint8_t endingCylinder;
        uint32_t startingLBA;
        uint32_t totalBlocks;
    } __attribute__((packed)) MBR_PARTITION;

    typedef struct {
        MBR_PARTITION partition;
        uint32_t ebrLBA;
    } EBR_PARTITION;

    typedef struct {
        uint8_t bootstrap[0x1BE];
        MBR_PARTITION partitions[0x4];
        uint16_t signature;
    } __attribute__((packed)) MASTER_BOOT_RECORD;

    typedef struct {
        uint64_t lowPart;
        uint64_t highPart;
    } __attribute__((packed)) EFI_GUID;

    typedef struct {
        char signature[0x08];
        uint32_t revision;
        uint32_t headerSize;
        uint32_t headerCRC32;
        uint32_t reserved;
        uint64_t myLBA;
        uint64_t alternateLBA;
        uint64_t firstUsableLBA;
        uint64_t lastUsableLBA;
        EFI_GUID diskGUID;
        uint64_t partitionEntryLBA;
        uint32_t partitionCount;
        uint32_t partitionEntrySize;
        uint32_t partitionEntryCRC32;
        uint8_t reserved2[0x1A4];
    } __attribute__((packed)) GPT_HEADER;

    typedef struct {
        EFI_GUID partitionTypeGUID;
        EFI_GUID uniquePartitionGUID;
        uint64_t startingLBA;
        uint64_t endingLBA;
        uint64_t attributes;
        char partitionName[0x48];
    } __attribute__((packed)) GPT_PARTITION_ENTRY;

    explicit PartitionTableReader(const DiscInterface *discInterface);

    const MASTER_BOOT_RECORD &getMbr() const;
    const std::vector<MBR_PARTITION> &getMbrPartitions() const;
    const std::vector<EBR_PARTITION> &getEbrPartitions() const;

    bool hasGPT();

    bool isReady() const;
private:
    bool readMbrFromDisc(const DiscInterface *discInterface);
    bool detectProtectiveMbr(const DiscInterface *discInterface);
    bool readGptFromDisc(const DiscInterface *discInterface, uint32_t headerSector);
    void detectExtendedPartitions(const DiscInterface *discInterface);

    bool isExtendedPartitionType(uint8_t partitionType);
    void discoverExtendedPartition(const DiscInterface *discInterface,
                                          const MBR_PARTITION &extendedPartitionEntry,
                                          std::vector<EBR_PARTITION> *logicalPartitions);

    void correctMBRPartitionEndianness(MBR_PARTITION &partition);
    void correctGptHeaderEndianness(GPT_HEADER &header);

    MASTER_BOOT_RECORD m_mbr;
    std::vector<MBR_PARTITION> m_mbrPartitions;
    std::vector<EBR_PARTITION> m_ebrPartitions;

    bool m_gptPresent;
    GPT_HEADER m_gptHeader;
    std::vector<GPT_PARTITION_ENTRY> m_gptPartitionEntries;

    bool m_ready;
};


#endif //UFILE_CPP_PARTITIONTABLEREADER_H
