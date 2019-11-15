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
#include <DiscInterface/DiscInterface.hpp>
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

    explicit PartitionTableReader(const DiscInterface *discInterface);

    const MASTER_BOOT_RECORD &getMbr() const;
    const std::vector<MBR_PARTITION> &getMbrPartitions() const;
    const std::vector<EBR_PARTITION> &getEbrPartitions() const;

    bool hasGPT();

    bool isReady() const;

    static bool isExtendedPartitionType(const uint8_t partitionType);

private:
    static void discoverExtendedPartition(const DiscInterface *discInterface,
                                          const MBR_PARTITION &extendedPartitionEntry,
                                          std::vector<EBR_PARTITION> *logicalPartitions);

    static void correctMBRPartitionEndianness(MBR_PARTITION *partition);

    MASTER_BOOT_RECORD m_mbr;
    std::vector<MBR_PARTITION> m_mbrPartitions;
    std::vector<EBR_PARTITION> m_ebrPartitions;

    bool m_gptPresent;

    bool m_ready;
};


#endif //UFILE_CPP_PARTITIONTABLEREADER_H
