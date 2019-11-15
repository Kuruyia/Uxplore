#include "EntryPhysicalMountedPartition.hpp"

EntryPhysicalMountedPartition::EntryPhysicalMountedPartition(const std::shared_ptr<PhysicalDevice> &device, const std::shared_ptr<MountedPartition> &partition)
: m_device(device)
, m_partition(partition)
{

}

std::string EntryPhysicalMountedPartition::getText() const {
    return m_partition->getName();
}

Entry::EntryType EntryPhysicalMountedPartition::getType() const {
    return ENTRY_PARTITION;
}

std::string EntryPhysicalMountedPartition::getIconName() const {
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

std::unique_ptr<Entry> EntryPhysicalMountedPartition::clone() const {
    return std::make_unique<EntryPhysicalMountedPartition>(*this);
}
