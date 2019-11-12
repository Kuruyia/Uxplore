#ifndef PHYSICALDEVICE_H
#define PHYSICALDEVICE_H

#include <Device/PhysicalDevice/PartitionTableReader.hpp>
#include <memory>
#include "../DiscInterface/DiscInterface.hpp"

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

    std::string getDeviceName();
	std::string getDeviceId();
	std::string getDevicePath();

    const std::unique_ptr<PartitionTableReader> &getPartitionTableReader() const;

    bool isPartitionTableAvailable() const;

private:
	DiscInterface m_discInterface;

	std::string m_name;
	std::string m_id;
	Filesystem m_filesystem;
	DeviceType m_deviceType;

	std::unique_ptr<PartitionTableReader> m_partitionTableReader;
	bool m_partitionTableAvailable;
};

#endif // PHYSICALDEVICE_H