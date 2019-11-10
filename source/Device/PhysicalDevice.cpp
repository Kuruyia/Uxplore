#include "PhysicalDevice.hpp"

PhysicalDevice::PhysicalDevice(const std::string &deviceId)
: m_discInterface("/dev/" + deviceId)
, m_id(deviceId)
, m_name("<name not set>")
, m_filesystem(Filesystem::Unknown)
, m_deviceType(DeviceType::Generic)
{
    m_discInterface.startup();
}

PhysicalDevice::~PhysicalDevice() {
    m_discInterface.shutdown();
}

std::shared_ptr<File> PhysicalDevice::getFile(std::string path) {

}

std::vector<std::shared_ptr<File>> PhysicalDevice::listFolder(std::string path) {

}

void PhysicalDevice::writeFile(std::string path) {

}

void PhysicalDevice::readFile(std::string path) {

}

void PhysicalDevice::deleteEntry(std::string path) {

}

void PhysicalDevice::createFolder(std::string path) {

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
