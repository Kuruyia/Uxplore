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

#ifndef PHYSICALDEVICE_H
#define PHYSICALDEVICE_H

#include "Device/PhysicalDevice/MountedPartition.h"
#include "Device/PhysicalDevice/PartitionTableReader.h"
#include "DiscInterface/DiscInterface.h"

#include <memory>

class PhysicalDevice {
public:
    enum DeviceType {
        Generic,
        Disc,
        USB,
        SD
    };

    PhysicalDevice(const std::string &deviceId, const bool &skipPartitionTableRead);
    ~PhysicalDevice();

    DiscInterface *getDiscInterface();

    DeviceType getDeviceType() const;
    void setDeviceType(DeviceType deviceType);

    const std::string &getDeviceId() const;
    std::string getDevicePath() const;

    const std::unique_ptr<PartitionTableReader> &getPartitionTableReader() const;
    bool isPartitionTableAvailable() const;

    const std::vector<std::shared_ptr<MountedPartition>> &getMountedPartitions() const;
    const std::shared_ptr<MountedPartition> &getMountedPartition(std::size_t pos) const;
    void addMountedPartition(const std::shared_ptr<MountedPartition> &newPartition);
    void removeMountedPartition(unsigned pos);

private:
    DiscInterface m_discInterface;

    std::string m_id;
    DeviceType m_deviceType;

    std::unique_ptr<PartitionTableReader> m_partitionTableReader;
    bool m_partitionTableAvailable;
    std::vector<std::shared_ptr<MountedPartition>> m_mountedPartitions;
};

#endif // PHYSICALDEVICE_H