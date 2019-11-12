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
            // Not a protective MBR, discover EBRs instead
            std::vector<EBR_PARTITION> discoveredEBRs;
            discoverEBRPartitions(discInterface, m_mbrPartitions, &discoveredEBRs);

            WHBLogPrintf("Discovered %u EBRs", discoveredEBRs.size());
        }
    } else {
        WHBLogPrintf("PartitionTableReader: Error while reading first sector");
    }
}

void PartitionTableReader::discoverEBRPartitions(const DiscInterface *discInterface,
                                                 const std::vector<MBR_PARTITION> &mbrPartitions,
                                                 std::vector<EBR_PARTITION> *ebrPartitions) {
    for (const MBR_PARTITION & partition : mbrPartitions) {
        if (partition.partitionType == 0x05 || partition.partitionType == 0x0F || partition.partitionType == 0x85) {
            // Partition is an EBR
            EBR_PARTITION currentEBR, nextEBR;
            MASTER_BOOT_RECORD ebr;
            uint32_t sectorToCheck = partition.startingLBA;

            // Only fill this information in currentEBR because nextEBR won't be kept
            currentEBR.firstRecordLBA = partition.startingLBA;

            do {
                if (!discInterface->readSectors(sectorToCheck, 1, &ebr))
                    break;

                if (ebr.signature != 0x55AA && ebr.signature != 0x55AB)
                    break;

                correctMBRPartitionEndianness(&ebr.partitions[0]);
                currentEBR.partition = ebr.partitions[0];
                ebrPartitions->push_back(currentEBR);

                correctMBRPartitionEndianness(&ebr.partitions[1]);
                nextEBR.partition = ebr.partitions[1];
                sectorToCheck = nextEBR.partition.startingLBA + partition.startingLBA;
            } while (nextEBR.partition.startingLBA != 0);
        }
    }
}

bool PartitionTableReader::hasGPT() {
    return m_gpt;
}

void PartitionTableReader::correctMBRPartitionEndianness(PartitionTableReader::MBR_PARTITION *partition) {
    partition->startingLBA = Utils::swapEndian32(partition->startingLBA);
    partition->totalBlocks = Utils::swapEndian32(partition->totalBlocks);
}
