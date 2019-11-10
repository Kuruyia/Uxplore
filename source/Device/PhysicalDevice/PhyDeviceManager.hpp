#ifndef PHYDEVICEMANAGER_H
#define PHYDEVICEMANAGER_H

#include "../PhysicalDevice.hpp"

#include <memory>
#include <string>
#include <vector>

class PhysicalDeviceManager {
public:
	PhysicalDeviceManager();
	~PhysicalDeviceManager();

	bool update();

    std::vector<std::string> getMountedDevicesId();
    std::vector<std::shared_ptr<PhysicalDevice>> getMoutedDevices();

private:
    enum FSMountCandidates {
        Native = 1 << 0,
        FAT    = 1 << 1,

        All    = Native | FAT
    };

    bool tryMountPartition(PhysicalDevice *physicalDevice, sec_t startSector, FSMountCandidates mountCandidates);
	bool tryMountNative(const std::string& deviceName);

	void unmountAll();

	unsigned int m_lastUpdate;
	int m_fsaFd;

	std::vector<std::pair<std::string, std::shared_ptr<PhysicalDevice>>> m_mountedDevices;
	std::vector<std::string> m_blacklist;
};

#endif // PHYDEVICEMANAGER_H