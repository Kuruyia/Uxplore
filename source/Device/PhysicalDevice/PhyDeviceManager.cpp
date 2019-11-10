#include "PhyDeviceManager.hpp"
#include "PhyDeviceUtils.hpp"
#include "PartitionTableReader.h"

#include <SDL2/SDL.h>
#include <whb/log.h>
#include <iosuhax.h>
#include <iosuhax_devoptab.h>
#include <fat.h>

#include <dirent.h>

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

		std::vector<std::string> addedDevices, removedDevices, removedBlacklistedDevices;
		PhysicalDeviceUtils::getDeviceDelta(getMountedDevicesId(), m_blacklist, &addedDevices, &removedDevices, &removedBlacklistedDevices);

		for (auto & addedDevice : addedDevices) {
			WHBLogPrintf("Device added - %s", addedDevice.c_str());
			bool isMounted;

            std::shared_ptr<PhysicalDevice> deviceToMount(new PhysicalDevice(addedDevice));

            if (!PhysicalDeviceUtils::isNative(addedDevice)) {
                // Added device is NOT native-exclusive

                // TODO: Finish partition support
                PartitionTableReader partitionTableReader(deviceToMount->getDiscInterface());

                isMounted = tryMountPartition(deviceToMount.get(), 0, FSMountCandidates::All);
            } else {
                // Added device is native-exclusive
                isMounted = tryMountPartition(deviceToMount.get(), 0, FSMountCandidates::Native);
            }

            if (isMounted) {
                WHBLogPrintf("Device mounted - %s", addedDevice.c_str());

                // Guess and set device type
                if (addedDevice.compare(0, 3, "odd") == 0)
                    deviceToMount->setDeviceType(PhysicalDevice::DeviceType::Disc);
                else if (addedDevice.compare(0, 3, "usb") == 0)
                    deviceToMount->setDeviceType(PhysicalDevice::DeviceType::USB);
                else if (addedDevice.compare(0, 6, "sdcard") == 0)
                    deviceToMount->setDeviceType(PhysicalDevice::DeviceType::SD);

                m_mountedDevices.emplace_back(deviceToMount->getDeviceId(), deviceToMount);
                hasChanged = true;
            } else {
                WHBLogPrintf("Device failed to mount - %s", addedDevice.c_str());

                m_blacklist.push_back(addedDevice);
            }
		}

		for (auto & removedDevice : removedDevices) {
			WHBLogPrintf("Device removed - %s", removedDevice.c_str());

			for (auto it = m_mountedDevices.begin(); it != m_mountedDevices.end(); it++) {
				if (it->first == removedDevice) {
					WHBLogPrintf("Confirmed device removed - %s", it->first.c_str());

					// TODO: Use fatUnmount()
					unmount_fs(it->second->getDeviceId().c_str());

					m_mountedDevices.erase(it);

					hasChanged = true;
					break;
				}
			}
		}

		for (auto & removedBlacklistedDevice : removedBlacklistedDevices) {
			WHBLogPrintf("Blacklisted device removed - %s", removedBlacklistedDevice.c_str());

			for (auto it = m_blacklist.begin(); it != m_blacklist.end(); it++) {
				if (*it == removedBlacklistedDevice) {
					WHBLogPrintf("Confirmed blacklisted device removed - %s", it->c_str());
					m_blacklist.erase(it);

					hasChanged = true;
					break;
				}
			}
		}

		if (hasChanged)
			WHBLogPrint("===== DEVICES CHANGED =====");

		return hasChanged;
	}

	return false;
}

bool PhysicalDeviceManager::tryMountPartition(PhysicalDevice *physicalDevice, sec_t startSector,
                                              FSMountCandidates mountCandidates) {
    const std::string& devId = physicalDevice->getDeviceId();

    if (mountCandidates & FSMountCandidates::FAT && fatMountSimple(devId.c_str(), physicalDevice->getDiscInterface()->getInterface())) {
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

        char deviceName[11];
        fatGetVolumeLabel(devId.c_str(), deviceName);
        PhysicalDeviceUtils::shrinkFATLabel(deviceName);
		physicalDevice->setDeviceName(deviceName);

		physicalDevice->setFilesystem(PhysicalDevice::Filesystem::FAT);

		return true;
	}

    if (mountCandidates & FSMountCandidates::Native && tryMountNative(devId)) {
        // TODO: Show devId more cleanly
        physicalDevice->setDeviceName(PhysicalDeviceUtils::getNativeDeviceName(devId) + " (" + devId + ")");
        physicalDevice->setFilesystem(PhysicalDevice::Filesystem::Native);

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

std::vector<std::string> PhysicalDeviceManager::getMountedDevicesId() {
	std::vector<std::string> deviceIds;

	for (auto & m_mountedDevice : m_mountedDevices) {
		WHBLogPrintf("MOUNTED ID - %s", m_mountedDevice.first.c_str());
		deviceIds.push_back(m_mountedDevice.first);
	}

	return deviceIds;
}

std::vector<std::shared_ptr<PhysicalDevice>> PhysicalDeviceManager::getMoutedDevices() {
    std::vector<std::shared_ptr<PhysicalDevice>> devices;

    for (auto &m_mountedDevice : m_mountedDevices) {
        devices.push_back(m_mountedDevice.second);
    }

    return devices;
}

void PhysicalDeviceManager::unmountAll() {
    WHBLogPrint("Unmounting everything");
    // TODO: Unmount FAT devices with fatUnmount()
    for (auto & m_mountedDevice : m_mountedDevices) {
        unmount_fs(m_mountedDevice.second->getDeviceId().c_str());
    }
}
