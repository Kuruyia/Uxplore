/*
    Uxplore
    Copyright (C) 2019-2020, Kuruyia

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

#include "PhyDeviceUtils.h"

#include <iosuhax.h>

#include <algorithm>
#include <cstring>
#include <set>

const char *m_whitelistedDevices[] = {"sdcard", "usb", "odd"};

int PhysicalDeviceUtils::getMountableDevices(std::vector<std::string> &devices)
{
    int folderHandle, ret;
    int fsaHandle = IOSUHAX_FSA_Open();

    if (fsaHandle < 0)
        return fsaHandle;

    if ((ret = IOSUHAX_FSA_OpenDir(fsaHandle, "/dev", &folderHandle)) >= 0) {
        directoryEntry_s entry;

        while (IOSUHAX_FSA_ReadDir(fsaHandle, folderHandle, &entry) == 0) {
            if (isStringInArray(entry.name, m_whitelistedDevices,
                                sizeof(m_whitelistedDevices) / sizeof(m_whitelistedDevices[0])))
                devices.emplace_back(std::string(entry.name));
        }
    } else {
        IOSUHAX_FSA_Close(fsaHandle);
        return ret;
    }

    IOSUHAX_FSA_Close(fsaHandle);
    return 0;
}

int PhysicalDeviceUtils::getDeviceDelta(std::vector<std::string> mountedDevices, std::vector<std::string> &addedDevices,
                                        std::vector<std::string> &removedDevices)
{
    std::vector<std::string> allMountableDevices;
    int ret;

    if ((ret = getMountableDevices(allMountableDevices)) < 0)
        return ret;

    // Need to sort vectors for set_difference to work properly
    std::sort(mountedDevices.begin(), mountedDevices.end());
    std::sort(allMountableDevices.begin(), allMountableDevices.end());

    // Check differences between mounted and available devices
    std::set_difference(allMountableDevices.begin(), allMountableDevices.end(), mountedDevices.begin(),
                        mountedDevices.end(), std::inserter(addedDevices, addedDevices.begin()));
    std::set_difference(mountedDevices.begin(), mountedDevices.end(), allMountableDevices.begin(),
                        allMountableDevices.end(), std::inserter(removedDevices, removedDevices.begin()));

    return 0;
}

bool PhysicalDeviceUtils::isStringInArray(const char *string, const char *array[], int arraySize)
{
    for (int i = 0; i < arraySize; i++) {
        if (strncmp(string, array[i], strlen(array[i])) == 0)
            return true;
    }

    return false;
}

bool PhysicalDeviceUtils::isNative(const std::string &id)
{
    return (startsWith(id, "odd")
            || startsWith(id, "mlc")
            || startsWith(id, "slc"));
}

std::string PhysicalDeviceUtils::getNativeDeviceName(const std::string &id)
{
    if (id == "odd01")
        return "Disc tickets";
    else if (id == "odd02")
        return "Disc update";
    else if (id == "odd03" || id == "odd04")
        return "Disc content";
    else if (id == "slccmpt01")
        return "NAND SLC (vWii compatible)";
    else if (id == "slc01")
        return "NAND SLC";
    else if (id == "mlc01")
        return "NAND MLC";

    return id;
}

void PhysicalDeviceUtils::shrinkFATLabel(char *label)
{
    unsigned i = 11;
    while (--i > 0) {
        if (label[i] == 0x20)
            label[i] = 0x00;
        else
            break;
    }
}

bool PhysicalDeviceUtils::startsWith(const std::string &sourceString, const std::string &startString)
{
    return startString.length() <= sourceString.length()
           && equal(startString.begin(), startString.end(), sourceString.begin());
}
