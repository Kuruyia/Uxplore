#ifndef DISCINTERFACE_HPP
#define DISCINTERFACE_HPP

#include <string>
#include <iosuhax_disc_interface.h>

class DiscInterface {
public:
	explicit DiscInterface(std::string devicePath);

	DISC_INTERFACE* getInterface();

	bool startup();
	bool isInserted() const;
	bool clearStatus();
	bool shutdown();
	bool readSectors(uint32_t sector, uint32_t numSectors, void* buffer) const;
	bool writeSectors(uint32_t sector, uint32_t numSectors, const void* buffer);

private:
	bool openFSA();
	void closeFSA();

	int m_fsaFdUSB;
	int m_usbFd;

	std::string m_devicePath;
	DISC_INTERFACE m_discInterface{};
};

#endif // DISCINTERFACE_HPP