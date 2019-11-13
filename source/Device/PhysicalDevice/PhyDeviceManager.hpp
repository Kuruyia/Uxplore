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

    std::vector<std::string> getInsertedDevicesId();
    std::vector<std::shared_ptr<PhysicalDevice>> getInsertedDevices();

private:
    enum FSMountCandidates {
        Native = 1 << 0,
        FAT    = 1 << 1,

        All    = Native | FAT
    };

    bool tryMountPartition(PhysicalDevice *physicalDevice, const std::string &partitionName,
                           sec_t startSector, FSMountCandidates mountCandidates,
                           MountedPartition::Filesystem *mountedFilesystem);
	bool tryMountNative(const std::string& deviceName);

	void unmountAll();

	static void updateMountedPartitionName(const std::shared_ptr<MountedPartition>& partition);

	unsigned int m_lastUpdate;
	int m_fsaFd;

	std::vector<std::pair<std::string, std::shared_ptr<PhysicalDevice>>> m_insertedDevices;
};

#endif // PHYDEVICEMANAGER_H