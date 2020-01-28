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

#include "PhyDeviceManager.h"
#include "PhyDeviceUtils.h"
#include "PartitionTableReader.h"

#include <SDL2/SDL.h>
#include <whb/log.h>
#include <iosuhax.h>
#include <iosuhax_devoptab.h>
#include <fat.h>

#include <dirent.h>

#define DEFAULT_CACHE_PAGES 4
#define DEFAULT_SECTORS_PAGE 64

PhysicalDeviceManager::PhysicalDeviceManager()
        : m_lastUpdate(0)
{
    m_fsaFd = IOSUHAX_FSA_Open();
}

PhysicalDeviceManager::~PhysicalDeviceManager()
{
    IOSUHAX_FSA_Close(m_fsaFd);
    unmountAll();
}

bool PhysicalDeviceManager::update()
{
    if (SDL_TICKS_PASSED(SDL_GetTicks(), m_lastUpdate + 2000)) {
        WHBLogPrint("Starting Phy discovery");

        m_lastUpdate = SDL_GetTicks();
        bool hasChanged = false;

        std::vector<std::string> addedDevices, removedDevices;
        int ret = PhysicalDeviceUtils::getDeviceDelta(getInsertedDevicesId(), &addedDevices, &removedDevices);
        if (ret < 0) {
            WHBLogPrintf("getDeviceDelta has failed with %i", ret);
            return false;
        }

        for (auto &addedDevice : addedDevices) {
            WHBLogPrintf("Device inserted - %s", addedDevice.c_str());
            bool isNative = PhysicalDeviceUtils::isNative(addedDevice);

            std::shared_ptr<PhysicalDevice> newDevice(new PhysicalDevice(addedDevice, isNative));
            updateDeviceType(newDevice);
            m_insertedDevices.emplace_back(newDevice->getDeviceId(), newDevice);

            if (newDevice->isPartitionTableAvailable()) {
                // Partition table available, we can read from it
                unsigned partitionCount = 1;
                if (newDevice->getPartitionTableReader()->hasGPT()) {
                    // Read the GPT
                    // TODO: Support GPT
                } else {
                    // Read the MBR/EBRs
                    for (const PartitionTableReader::MBR_PARTITION &partition : newDevice->getPartitionTableReader()->getMbrPartitions()) {
                        if (tryMountPartitionAndAddToDevice(newDevice, newDevice->getDeviceId() + "p" +
                                                                       std::to_string(partitionCount),
                                                            partition.startingLBA)) {
                            ++partitionCount;
                        }
                    }

                    for (const PartitionTableReader::EBR_PARTITION &partition : newDevice->getPartitionTableReader()->getEbrPartitions()) {
                        if (tryMountPartitionAndAddToDevice(newDevice, newDevice->getDeviceId() + "p" +
                                                                       std::to_string(partitionCount),
                                                            partition.ebrLBA + partition.partition.startingLBA)) {
                            ++partitionCount;
                        }
                    }
                }
            } else {
                // Partition table not available, we have to rely on the system
                MountedPartition::Filesystem mountedFilesystem;
                bool success = tryMountPartition(newDevice.get(), newDevice->getDeviceId(), 0,
                                                 FSMountCandidates::Native, &mountedFilesystem);

                if (success) {
                    std::shared_ptr<MountedPartition> newPartition(
                            new MountedPartition(newDevice->getDeviceId(), mountedFilesystem));

                    newDevice->addMountedPartition(newPartition);
                    updateMountedPartitionName(newPartition);
                }
            }

            hasChanged = true;
        }

        for (auto &removedDevice : removedDevices) {
            WHBLogPrintf("Device removed - %s", removedDevice.c_str());

            for (auto it = m_insertedDevices.begin(); it != m_insertedDevices.end(); it++) {
                if (it->first == removedDevice) {
                    WHBLogPrintf("Found removed device - %s", it->first.c_str());

                    for (const std::shared_ptr<MountedPartition> &partition : it->second->getMountedPartitions()) {
                        unmountPartition(partition);
                    }

                    m_insertedDevices.erase(it);

                    hasChanged = true;
                    break;
                }
            }
        }

        if (hasChanged) {
            WHBLogPrint("===== Device list changed =====");
            for (const std::pair<std::basic_string<char>, std::shared_ptr<PhysicalDevice>> &device : m_insertedDevices) {
                WHBLogPrintf("Inserted device: %s", device.first.c_str());
            }
        }

        return hasChanged;
    }

    return false;
}

bool PhysicalDeviceManager::tryMountPartition(PhysicalDevice *physicalDevice, const std::string &partitionName,
                                              sec_t startSector, FSMountCandidates mountCandidates,
                                              MountedPartition::Filesystem *mountedFilesystem)
{
    if (mountCandidates & FSMountCandidates::FAT &&
        fatMount(partitionName.c_str(), physicalDevice->getDiscInterface()->getInterface(), startSector,
                 DEFAULT_CACHE_PAGES, DEFAULT_SECTORS_PAGE)) {
        // TODO: Remove this
        /*WHBLogPrint("");
        WHBLogPrintf("LISTING DEVICE %s", physicalDevice->getDeviceId().c_str());

        DIR *pdir;
        struct dirent *pent;

        pdir=opendir((physicalDevice->getDeviceId() + ":/").c_str());

        WHBLogPrint("dir opened");

        if (pdir) {
            while ((pent=readdir(pdir))!=nullptr) {
                if(strcmp(".", pent->d_name) == 0 || strcmp("..", pent->d_name) == 0)
                    continue;
                if(pent->d_type == DT_DIR)
                    WHBLogPrintf("[%s]", pent->d_name);
                else
                    WHBLogPrintf("%s", pent->d_name);
            }
            closedir(pdir);
        } else {
            WHBLogPrintf("opendir() failure; terminating");
        }

        WHBLogPrint("END LISTING");
        WHBLogPrint("");*/

        *mountedFilesystem = MountedPartition::Filesystem::FAT;

        return true;
    }

    if (mountCandidates & FSMountCandidates::Native && tryMountNative(partitionName)) {
        *mountedFilesystem = MountedPartition::Filesystem::Native;

        return true;
    }

    return false;
}

bool PhysicalDeviceManager::tryMountNative(const std::string &deviceName)
{
    WHBLogPrintf("Trying to mount %s natively", deviceName.c_str());

    if (m_fsaFd < 0) return false;

    fileStat_s fileStat;
    if (IOSUHAX_FSA_GetStat(m_fsaFd, ("/vol/storage_" + deviceName).c_str(), &fileStat) < 0) return false;

    int ret = mount_fs(deviceName.c_str(), m_fsaFd, nullptr, ("/vol/storage_" + deviceName).c_str());
    return ret >= 0;
}

std::vector<std::string> PhysicalDeviceManager::getInsertedDevicesId()
{
    std::vector<std::string> deviceIds;

    for (auto &m_mountedDevice : m_insertedDevices) {
        deviceIds.push_back(m_mountedDevice.first);
    }

    return deviceIds;
}

std::vector<std::shared_ptr<PhysicalDevice>> PhysicalDeviceManager::getInsertedDevices()
{
    std::vector<std::shared_ptr<PhysicalDevice>> devices;

    for (auto &m_mountedDevice : m_insertedDevices) {
        devices.push_back(m_mountedDevice.second);
    }

    return devices;
}

void PhysicalDeviceManager::unmountAll()
{
    WHBLogPrint("Unmounting everything");
    // TODO: Unmount all partitions of all devices
}

void PhysicalDeviceManager::updateMountedPartitionName(const std::shared_ptr<MountedPartition> &partition)
{
    switch (partition->getFilesystem()) {
        case MountedPartition::Filesystem::FAT:
            char deviceName[11];
            fatGetVolumeLabel(partition->getId().c_str(), deviceName);
            PhysicalDeviceUtils::shrinkFATLabel(deviceName);
            partition->setName(deviceName);

            break;
        case MountedPartition::Filesystem::Native:
            // TODO: Show devId more cleanly
            partition->setName(
                    PhysicalDeviceUtils::getNativeDeviceName(partition->getId()) + " (" + partition->getId() + ")");

            break;
        default:
            WHBLogPrintf("PhysicalDeviceManager::updateMountedPartitionName - Unhandled partition type");
            break;
    }
}

void PhysicalDeviceManager::updateDeviceType(const std::shared_ptr<PhysicalDevice> &device)
{
    const std::string deviceId = device->getDeviceId();
    if (deviceId.compare(0, 3, "odd") == 0)
        device->setDeviceType(PhysicalDevice::DeviceType::Disc);
    else if (deviceId.compare(0, 3, "usb") == 0)
        device->setDeviceType(PhysicalDevice::DeviceType::USB);
    else if (deviceId.compare(0, 6, "sdcard") == 0)
        device->setDeviceType(PhysicalDevice::DeviceType::SD);
}

bool PhysicalDeviceManager::tryMountPartitionAndAddToDevice(std::shared_ptr<PhysicalDevice> &device,
                                                            const std::string &partitionName,
                                                            sec_t startSector)
{
    MountedPartition::Filesystem mountedFilesystem;
    if (tryMountPartition(device.get(), partitionName, startSector, FSMountCandidates::All, &mountedFilesystem)) {
        std::shared_ptr<MountedPartition> newPartition(new MountedPartition(partitionName, mountedFilesystem));

        device->addMountedPartition(newPartition);
        updateMountedPartitionName(newPartition);

        return true;
    }

    return false;
}

void PhysicalDeviceManager::unmountPartition(const std::shared_ptr<MountedPartition> &partition)
{
    switch (partition->getFilesystem()) {
        case MountedPartition::Filesystem::FAT:
            fatUnmount(partition->getId().c_str());

            break;
        case MountedPartition::Filesystem::Native:
            unmount_fs(partition->getId().c_str());

            break;
        default:
            WHBLogPrintf("PhysicalDeviceManager::unmountPartition - Unhandled partition type");
            break;
    }
}
