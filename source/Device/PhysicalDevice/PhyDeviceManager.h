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

#ifndef PHYDEVICEMANAGER_H
#define PHYDEVICEMANAGER_H

#include <memory>
#include <string>
#include <vector>

#include "../PhysicalDevice.h"

class PhysicalDeviceManager
{
public:
    PhysicalDeviceManager();

    ~PhysicalDeviceManager();

    bool update();

    std::vector<std::string> getInsertedDevicesId() const;

    std::vector<std::shared_ptr<PhysicalDevice>> getInsertedDevices() const;

private:
    enum FSMountCandidates {
        Native = 1 << 0,
        FAT = 1 << 1,

        All = Native | FAT
    };

    bool tryMountPartition(std::shared_ptr<PhysicalDevice> &physicalDevice, const std::string &partitionName,
                           const sec_t &startSector, const FSMountCandidates &mountCandidates,
                           MountedPartition::Filesystem &mountedFilesystem);
    bool tryMountNative(const std::string &deviceName);
    bool tryMountPartitionAndAddToDevice(std::shared_ptr<PhysicalDevice> &device, const std::string &partitionName,
                                         sec_t startSector);

    void unmountPartition(const std::shared_ptr<PhysicalDevice> &device, const std::size_t &pos);
    void unmountDevice(const std::shared_ptr<PhysicalDevice> &device);
    void unmountAll();

    static void updateMountedPartitionName(const std::shared_ptr<MountedPartition> &partition);
    static void updateDeviceType(const std::shared_ptr<PhysicalDevice> &device);

    unsigned int m_lastUpdate;
    int m_fsaFd;

    std::vector<std::pair<std::string, std::shared_ptr<PhysicalDevice>>> m_insertedDevices;
};

#endif // PHYDEVICEMANAGER_H