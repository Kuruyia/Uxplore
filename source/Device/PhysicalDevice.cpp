/*
    Uxplore
    Copyright (C) 2019-2019, Kuruyia

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "PhysicalDevice.h"

#include <memory>

PhysicalDevice::PhysicalDevice(const std::string &deviceId, const bool &skipPartitionTableRead)
: m_discInterface("/dev/" + deviceId)
, m_id(deviceId)
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

const std::string &PhysicalDevice::getDeviceId() const {
	return m_id;
}

std::string PhysicalDevice::getDevicePath() const {
	return "/dev/" + m_id;
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
    m_mountedPartitions.emplace_back(newPartition);
}

void PhysicalDevice::removeMountedPartition(unsigned pos) {
    m_mountedPartitions.erase(m_mountedPartitions.begin() + pos);
}
