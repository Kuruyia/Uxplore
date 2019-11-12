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
        uint32_t startingLBA;
        uint32_t totalBlocks;
    } MBR_PARTITION;

    typedef struct {
        MBR_PARTITION partition;
        uint32_t firstRecordLBA;
    } EBR_PARTITION;

    explicit PartitionTableReader(const DiscInterface* discInterface);

    bool hasGPT();

private:
    static void discoverEBRPartitions(const DiscInterface *discInterface,
                               const std::vector<MBR_PARTITION> &mbrPartitions,
                               std::vector<EBR_PARTITION> *ebrPartitions);

    static MBR_PARTITION getMBRPartitionFromData(uint8_t *data);

    int m_returnCode;
    bool m_gpt;

    std::vector<MBR_PARTITION> m_mbrPartitions;
};


#endif //UFILE_CPP_PARTITIONTABLEREADER_H
