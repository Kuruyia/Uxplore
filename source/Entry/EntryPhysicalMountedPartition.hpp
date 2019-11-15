#ifndef UXPLORE_CPP_ENTRYPHYSICALMOUNTEDPARTITION_HPP
#define UXPLORE_CPP_ENTRYPHYSICALMOUNTEDPARTITION_HPP

#include "../Device/PhysicalDevice.hpp"
#include "../Device/PhysicalDevice/MountedPartition.hpp"
#include "Entry.hpp"

class EntryPhysicalMountedPartition: public Entry {
public:
    EntryPhysicalMountedPartition(const std::shared_ptr<PhysicalDevice> &device, const std::shared_ptr<MountedPartition> &partition);

    std::string getText() const override;
    EntryType getType() const override;
    std::string getIconName() const override;

    std::unique_ptr<Entry> clone() const override;

private:
    std::shared_ptr<PhysicalDevice> m_device;
    std::shared_ptr<MountedPartition> m_partition;
};


#endif //UXPLORE_CPP_ENTRYPHYSICALMOUNTEDPARTITION_HPP
