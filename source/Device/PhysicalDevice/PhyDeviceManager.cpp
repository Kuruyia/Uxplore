#include "PhyDeviceManager.hpp"
#include "PhyDeviceUtils.hpp"
#include "PartitionTableReader.hpp"

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

PhysicalDeviceManager::~PhysicalDeviceManager() {
    IOSUHAX_FSA_Close(m_fsaFd);
    unmountAll();
}

bool PhysicalDeviceManager::update() {
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

		for (auto & addedDevice : addedDevices) {
			WHBLogPrintf("Device inserted - %s", addedDevice.c_str());
			bool isNative = PhysicalDeviceUtils::isNative(addedDevice);

            std::shared_ptr<PhysicalDevice> newDevice(new PhysicalDevice(addedDevice, isNative));
            m_insertedDevices.emplace_back(newDevice->getDeviceId(), newDevice);

            if (newDevice->isPartitionTableAvailable()) {
                // Partition table available, we can read from it
            } else {
                // Partition table not available, we have to rely on the system
                MountedPartition::Filesystem mountedFilesystem;
                bool success = tryMountPartition(newDevice.get(), newDevice->getDeviceId(), 0,
                                                 FSMountCandidates::Native, &mountedFilesystem);

                if (success) {
                    newDevice->addMountedPartition(std::make_shared<MountedPartition>(MountedPartition(newDevice->getDeviceId(), mountedFilesystem)));
                }
            }

            hasChanged = true;

            /*if (!isNative) {
                // Added device is NOT native-exclusive
                isMounted = tryMountPartition(newDevice.get(), 0, FSMountCandidates::All);
            } else {
                // Added device is native-exclusive
                isMounted = tryMountPartition(newDevice.get(), 0, FSMountCandidates::Native);
            }

            if (isMounted) {
                WHBLogPrintf("Device mounted - %s", addedDevice.c_str());

                // Guess and set device type
                if (addedDevice.compare(0, 3, "odd") == 0)
                    newDevice->setDeviceType(PhysicalDevice::DeviceType::Disc);
                else if (addedDevice.compare(0, 3, "usb") == 0)
                    newDevice->setDeviceType(PhysicalDevice::DeviceType::USB);
                else if (addedDevice.compare(0, 6, "sdcard") == 0)
                    newDevice->setDeviceType(PhysicalDevice::DeviceType::SD);

                m_insertedDevices.emplace_back(newDevice->getDeviceId(), newDevice);
                hasChanged = true;
            } else {
                WHBLogPrintf("Device failed to mount - %s", addedDevice.c_str());

                m_blacklist.push_back(addedDevice);
            }*/
		}

		for (auto & removedDevice : removedDevices) {
			WHBLogPrintf("Device removed - %s", removedDevice.c_str());

			for (auto it = m_insertedDevices.begin(); it != m_insertedDevices.end(); it++) {
				if (it->first == removedDevice) {
					WHBLogPrintf("Confirmed device removed - %s", it->first.c_str());

					// TODO: Unmount every partition on device

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
                                              MountedPartition::Filesystem *mountedFilesystem) {
    if (mountCandidates & FSMountCandidates::FAT &&
            fatMount(partitionName.c_str(), physicalDevice->getDiscInterface()->getInterface(), startSector, DEFAULT_CACHE_PAGES, DEFAULT_SECTORS_PAGE)) {
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

		// TODO: Reimplement this for partition support
        /*char deviceName[11];
        fatGetVolumeLabel(partitionName.c_str(), deviceName);
        PhysicalDeviceUtils::shrinkFATLabel(deviceName);
		physicalDevice->setDeviceName(deviceName);*/

		*mountedFilesystem = MountedPartition::Filesystem ::FAT;

		return true;
	}

    if (mountCandidates & FSMountCandidates::Native && tryMountNative(partitionName)) {
        // TODO: Show devId more cleanly && Reimplement this for partition support
        //physicalDevice->setDeviceName(PhysicalDeviceUtils::getNativeDeviceName(partitionName) + " (" + partitionName + ")");
        *mountedFilesystem = MountedPartition::Filesystem::Native;

        return true;
    }

	return false;
}

bool PhysicalDeviceManager::tryMountNative(const std::string& deviceName) {
    WHBLogPrintf("Checking nativeness of %s", deviceName.c_str());

    if (m_fsaFd < 0) return false;

    fileStat_s fileStat;
    if (IOSUHAX_FSA_GetStat(m_fsaFd, ("/vol/storage_" + deviceName).c_str(), &fileStat) < 0) return false;

    int ret = mount_fs(deviceName.c_str(), m_fsaFd, nullptr, ("/vol/storage_" + deviceName).c_str());
    return ret >= 0;
}

std::vector<std::string> PhysicalDeviceManager::getInsertedDevicesId() {
	std::vector<std::string> deviceIds;

	for (auto & m_mountedDevice : m_insertedDevices) {
		deviceIds.push_back(m_mountedDevice.first);
	}

	return deviceIds;
}

std::vector<std::shared_ptr<PhysicalDevice>> PhysicalDeviceManager::getInsertedDevices() {
    std::vector<std::shared_ptr<PhysicalDevice>> devices;

    for (auto &m_mountedDevice : m_insertedDevices) {
        devices.push_back(m_mountedDevice.second);
    }

    return devices;
}

void PhysicalDeviceManager::unmountAll() {
    WHBLogPrint("Unmounting everything");
    // TODO: Unmount all partitions of all devices
}
