#ifndef UFILE_CPP_PARTITIONTABLEREADER_H
#define UFILE_CPP_PARTITIONTABLEREADER_H

#include <string>
#include <vector>
#include <DiscInterface/DiscInterface.hpp>

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
        uint32_t firstSector;
        uint32_t totalSectors;
    } MBR_partition;

    explicit PartitionTableReader(const DiscInterface* discInterface);

    bool hasGPT();

private:
    static void discoverEBRPartitions(const DiscInterface *discInterface,
                               const std::vector<MBR_partition> &mbrPartitions,
                               std::vector<MBR_partition> *ebrPartitions);

    int m_returnCode;
    bool m_gpt;

    std::vector<MBR_partition> m_mbrPartitions;
};


#endif //UFILE_CPP_PARTITIONTABLEREADER_H
