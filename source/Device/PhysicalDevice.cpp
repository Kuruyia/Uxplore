#include "PhysicalDevice.hpp"

#include <memory>

PhysicalDevice::PhysicalDevice(const std::string &deviceId, const bool &skipPartitionTableRead)
: m_discInterface("/dev/" + deviceId)
, m_id(deviceId)
, m_name("<name not set>")
, m_filesystem(Filesystem::Unknown)
, m_deviceType(DeviceType::Generic)
, m_partitionTableAvailable(!skipPartitionTableRead)
{
    m_discInterface.startup();

    if (!skipPartitionTableRead)
        m_partitionTableReader = std::make_unique<PartitionTableReader>(getDiscInterface());
}

PhysicalDevice::~PhysicalDevice() {
    m_discInterface.shutdown();
}

DiscInterface* PhysicalDevice::getDiscInterface() {
	return &m_discInterface;
}

std::string PhysicalDevice::getDeviceId() {
	return m_id;
}

std::string PhysicalDevice::getDevicePath() {
	return "/dev/" + m_id;
}

std::string PhysicalDevice::getDeviceName() {
    return m_name;
}

const std::string &PhysicalDevice::getDeviceName() const {
    return m_name;
}

void PhysicalDevice::setDeviceName(const std::string &deviceName) {
    m_name = deviceName;
}

PhysicalDevice::Filesystem PhysicalDevice::getFilesystem() const {
    return m_filesystem;
}

void PhysicalDevice::setFilesystem(PhysicalDevice::Filesystem filesystem) {
    m_filesystem = filesystem;
}

PhysicalDevice::DeviceType PhysicalDevice::getDeviceType() const {
    return m_deviceType;
}

void PhysicalDevice::setDeviceType(PhysicalDevice::DeviceType deviceType) {
    m_deviceType = deviceType;
}

const std::unique_ptr<PartitionTableReader> &PhysicalDevice::getPartitionTableReader() const {
    return m_partitionTableReader;
}

bool PhysicalDevice::isPartitionTableAvailable() const {
    return m_partitionTableAvailable && m_partitionTableReader->isReady();
}

const std::vector<std::shared_ptr<MountedPartition>> &PhysicalDevice::getMountedPartitions() const {
    return m_mountedPartitions;
}

void PhysicalDevice::addMountedPartition(const std::shared_ptr<MountedPartition>& newPartition) {
    m_mountedPartitions.push_back(newPartition);
}

void PhysicalDevice::removeMountedPartition(const unsigned pos) {
    m_mountedPartitions.erase(m_mountedPartitions.begin() + pos);
}
