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

#ifndef UXPLORE_CPP_ENTRYPHYSICALMOUNTEDPARTITION_H
#define UXPLORE_CPP_ENTRYPHYSICALMOUNTEDPARTITION_H

#include "Device/PhysicalDevice.h"
#include "Device/PhysicalDevice/MountedPartition.h"
#include "Entry.h"

class EntryPhysicalMountedPartition : public Entry {
public:
    EntryPhysicalMountedPartition(const std::shared_ptr<PhysicalDevice> &device,
                                  const std::shared_ptr<MountedPartition> &partition);

    ~EntryPhysicalMountedPartition() override = default;

    std::string getText() const override;
    EntryType getType() const override;
    std::string getIconName() const override;

    std::unique_ptr<Entry> clone() const override;

private:
    std::shared_ptr<PhysicalDevice> m_device;
    std::shared_ptr<MountedPartition> m_partition;
};


#endif //UXPLORE_CPP_ENTRYPHYSICALMOUNTEDPARTITION_H
