#ifndef PHYSICALDEVICE_H
#define PHYSICALDEVICE_H

#include "../DiscInterface/DiscInterface.hpp"
#include "FilesystemProvider.hpp"

class PhysicalDevice: public FilesystemProvider {
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

	PhysicalDevice(const std::string &deviceId);
	~PhysicalDevice();

	std::shared_ptr<File> getFile(std::string path) override;
	std::vector<std::shared_ptr<File>> listFolder(std::string path) override;

	void writeFile(std::string path) override;
	void readFile(std::string path) override;

	void deleteEntry(std::string path) override;

	void createFolder(std::string path) override;

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

private:
	DiscInterface m_discInterface;

	std::string m_name;
	std::string m_id;
	Filesystem m_filesystem;
	DeviceType m_deviceType;
};

#endif // PHYSICALDEVICE_H