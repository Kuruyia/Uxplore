#include "PartitionTableReader.h"

#include <cstring>
#include <whb/log.h>

#define MBR_SIG 0x1FE

PartitionTableReader::PartitionTableReader(const DiscInterface* discInterface)
: m_returnCode(-1)
, m_gpt(false)
{
    uint8_t mbrData[512];
    if (discInterface->readSectors(0, 1, &mbrData)) {
        uint16_t signature = ((uint16_t)mbrData[MBR_SIG] << 8) | mbrData[MBR_SIG + 0x01];

        WHBLogPrintf("PartitionTableReader: Succesfully read first sector");
        if (signature != 0x55AA && signature != 0x55AB) {
            WHBLogPrintf("PartitionTableReader: Invalid MBR signature found!");
            return;
        }

        // Extract partitions from MBR
        for (unsigned i = 0; i < 4; ++i) {
            MBR_partition partition;
            memcpy(&partition, &mbrData[0x1BE + (sizeof(MBR_partition) * i)], sizeof(MBR_partition));

            m_mbrPartitions.push_back(partition);
        }

        // Check if it's a protective MBR
        if (m_mbrPartitions[0].partitionType == 0xEE) {
            WHBLogPrintf("Protective MBR found!");
            m_gpt = true;
        } else {
            // Not a protective MBR, discover EBRs instead
            std::vector<MBR_partition> discoveredEBRs;
            discoverEBRPartitions(discInterface, m_mbrPartitions, &discoveredEBRs);

            WHBLogPrintf("Discovered %u EBRs", discoveredEBRs.size());
        }
    } else {
        WHBLogPrintf("PartitionTableReader: Error while reading first sector");
    }
}

bool PartitionTableReader::hasGPT() {
    return m_gpt;
}

void PartitionTableReader::discoverEBRPartitions(const DiscInterface *discInterface,
                                                 const std::vector<MBR_partition> &mbrPartitions,
                                                 std::vector<MBR_partition> *ebrPartitions) {
    for (const MBR_partition & partition : mbrPartitions) {
        if (partition.partitionType == 0x05 || partition.partitionType == 0x0F || partition.partitionType == 0x85) {
            // Partition is an EBR
            MBR_partition currentEBR, nextEBR;
            uint8_t ebrData[512];
            uint8_t sectorToCheck = partition.startingSector;

            do {
                if (!discInterface->readSectors(sectorToCheck, 1, &ebrData))
                    break;

                uint16_t signature = ((uint16_t)ebrData[MBR_SIG] << 8) | ebrData[MBR_SIG + 0x01];
                if (signature != 0x55AA && signature != 0x55AB)
                    break;

                memcpy(&currentEBR, &ebrData[0x1BE], sizeof(MBR_partition));
                currentEBR.startingSector += sectorToCheck;
                ebrPartitions->push_back(currentEBR);

                memcpy(&nextEBR, &ebrData[0x1CE], sizeof(MBR_partition));
                sectorToCheck += nextEBR.startingSector;
            } while (nextEBR.startingSector != 0);
        }
    }
}
