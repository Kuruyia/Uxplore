/*
    Uxplore
    Copyright (C) 2019-2021, Kuruyia

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

#include "EntryPhysicalMountedPartition.h"

EntryPhysicalMountedPartition::EntryPhysicalMountedPartition(const std::shared_ptr<PhysicalDevice> &device,
                                                             const std::shared_ptr<MountedPartition> &partition)
        : m_device(device)
        , m_partition(partition)
{

}

std::string EntryPhysicalMountedPartition::getText() const
{
    return m_partition->getName();
}

Entry::EntryType EntryPhysicalMountedPartition::getType() const
{
    return ENTRY_PARTITION;
}

std::string EntryPhysicalMountedPartition::getIconName() const
{
    switch (m_device->getDeviceType()) {
        case PhysicalDevice::Disc:
            return "devices/disc.png";
        case PhysicalDevice::USB:
            return "devices/usb.png";
        case PhysicalDevice::SD:
            return "devices/sd.png";
        default:
            return "devices/generic_fs.png";
    }
}

std::unique_ptr<Entry> EntryPhysicalMountedPartition::clone() const
{
    return std::make_unique<EntryPhysicalMountedPartition>(*this);
}
