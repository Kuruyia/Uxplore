#ifndef PHYSICALDEVICE_H
#define PHYSICALDEVICE_H

#include "PhysicalDevice/MountedPartition.hpp"
#include "PhysicalDevice/PartitionTableReader.hpp"
#include "../DiscInterface/DiscInterface.hpp"

#include <memory>

class PhysicalDevice {
public:
    enum Filesystem {
        Unknown,

        FAT,
        Native
    };

    enum DeviceType {
        Generic,
        Disc,
        USB,
        SD
    };

	PhysicalDevice(const std::string &deviceId, const bool &skipPartitionTableRead);
	~PhysicalDevice();

	DiscInterface* getDiscInterface();

    const std::string &getDeviceName() const;
    void setDeviceName(const std::string &deviceName);

    Filesystem getFilesystem() const;
    void setFilesystem(Filesystem filesystem);

    DeviceType getDeviceType() const;
    void setDeviceType(DeviceType deviceType);

	const std::string &getDeviceId() const;
	std::string getDevicePath() const;

    const std::unique_ptr<PartitionTableReader> &getPartitionTableReader() const;
    bool isPartitionTableAvailable() const;

    const std::vector<std::shared_ptr<MountedPartition>> &getMountedPartitions() const;
    void addMountedPartition(const std::shared_ptr<MountedPartition>& newPartition);
    void removeMountedPartition(unsigned pos);

private:
	DiscInterface m_discInterface;

	std::string m_name;
	std::string m_id;
	Filesystem m_filesystem;
	DeviceType m_deviceType;

	std::unique_ptr<PartitionTableReader> m_partitionTableReader;
	bool m_partitionTableAvailable;
	std::vector<std::shared_ptr<MountedPartition>> m_mountedPartitions;
};

#endif // PHYSICALDEVICE_H