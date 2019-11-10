#include "EntryPhyDevice.hpp"

EntryPhyDevice::EntryPhyDevice(PhysicalDevice *device)
: m_device(device)
{}

std::string EntryPhyDevice::getText() {
	return m_device->getDeviceName();
}

Entry::EntryType EntryPhyDevice::getType() {
	return EntryType::ENTRY_DEVICE;
}

std::string EntryPhyDevice::getIconName() {
	switch (m_device->getDeviceType()) {
        case PhysicalDevice::Disc:
            return "disc.png";
        case PhysicalDevice::USB:
            return "usb.png";
        case PhysicalDevice::SD:
            return "sd.png";
	    default:
	        return "generic_fs.png";
    }
}