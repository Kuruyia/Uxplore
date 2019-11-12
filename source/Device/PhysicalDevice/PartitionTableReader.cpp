#include "PartitionTableReader.hpp"
#include "Utils.hpp"

#include <cstring>
#include <whb/log.h>

#define MBR_SIG 0x1FE

PartitionTableReader::PartitionTableReader(const DiscInterface *discInterface)
: m_returnCode(-1)
, m_gpt(false)
{
    if (discInterface->readSectors(0, 1, &m_mbr)) {
        WHBLogPrintf("PartitionTableReader: Succesfully read first sector");
        if (m_mbr.signature != 0x55AA && m_mbr.signature != 0x55AB) {
            WHBLogPrintf("PartitionTableReader: Invalid MBR signature found!");
            return;
        }

        // Extract partitions from MBR
        m_mbrPartitions = std::vector<MBR_PARTITION>(std::begin(m_mbr.partitions), std::end(m_mbr.partitions));

        // We want little endian
        for (MBR_PARTITION& partition : m_mbrPartitions) {
            correctMBRPartitionEndianness(&partition);
        }

        // Check if it's a protective MBR
        if (m_mbrPartitions[0].partitionType == 0xEE) {
            // TODO: Check the GPT header and discover partitions
            WHBLogPrintf("Protective MBR found!");
            m_gpt = true;
        } else {
            // This is not a protective MBR, so we attempt to discover any extended partition
            for (const MBR_PARTITION& partition : m_mbrPartitions) {
                discoverExtendedPartition(discInterface, partition, &m_ebrPartitions);
            }

            WHBLogPrintf("Discovered %u EBRs", m_ebrPartitions.size());
        }
    } else {
        WHBLogPrintf("PartitionTableReader: Error while reading first sector");
    }
}

void PartitionTableReader::discoverExtendedPartition(const DiscInterface *discInterface,
                                                     const MBR_PARTITION &extendedPartitionEntry,
                                                     std::vector<EBR_PARTITION> *logicalPartitions) {
    // Check that the entry partition type is extended partition
    if (extendedPartitionEntry.partitionType != 0x05 && extendedPartitionEntry.partitionType != 0x0F && extendedPartitionEntry.partitionType != 0x85)
        return;

    EBR_PARTITION currentLogicalPartition;
    MBR_PARTITION nextEBR;
    MASTER_BOOT_RECORD ebr;
    uint32_t sectorToCheck = extendedPartitionEntry.startingLBA;

    // We need to keep track of this because an EBR entry starting LBA is relative to the first EBR LBA
    currentLogicalPartition.firstRecordLBA = extendedPartitionEntry.startingLBA;

    do {
        // Sanity checks
        if (!discInterface->readSectors(sectorToCheck, 1, &ebr))
            break;

        if (ebr.signature != 0x55AA && ebr.signature != 0x55AB)
            break;

        // Store first two entries
        correctMBRPartitionEndianness(&ebr.partitions[0]);
        currentLogicalPartition.partition = ebr.partitions[0];
        logicalPartitions->push_back(currentLogicalPartition);

        correctMBRPartitionEndianness(&ebr.partitions[1]);
        nextEBR = ebr.partitions[1];
        sectorToCheck = nextEBR.startingLBA + extendedPartitionEntry.startingLBA;
    } while (nextEBR.startingLBA != 0);
}

bool PartitionTableReader::hasGPT() {
    return m_gpt;
}

void PartitionTableReader::correctMBRPartitionEndianness(PartitionTableReader::MBR_PARTITION *partition) {
    partition->startingLBA = Utils::swapEndian32(partition->startingLBA);
    partition->totalBlocks = Utils::swapEndian32(partition->totalBlocks);
}
