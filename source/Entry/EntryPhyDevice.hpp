#ifndef ENTRYDEVICE_HPP
#define ENTRYDEVICE_HPP

#include <FilesystemProvider.hpp>
#include <Device/PhysicalDevice.hpp>
#include "Entry.hpp"

class EntryPhyDevice: public Entry {
public:
	EntryPhyDevice(PhysicalDevice *device);

	std::string getText() override;
	EntryType getType() override;
	std::string getIconName() override;

private:
    PhysicalDevice *m_device;
};

#endif // ENTRYDEVICE_HPP