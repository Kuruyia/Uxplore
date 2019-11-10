#include "PhyDeviceUtils.hpp"

#include <fat.h>
#include <iosuhax.h>
#include <sys/iosupport.h>
#include <whb/log.h>

#include <algorithm>
#include <cstring>
#include <iostream>
#include <iterator>
#include <set>

const char* m_whitelistedDevices[] = {"sdcard", "usb", "odd"};

int PhysicalDeviceUtils::getMountableDevices(std::vector<std::string>* devices) {
	int folderHandle, ret;
	int fsaHandle = IOSUHAX_FSA_Open();

	if (fsaHandle < 0)
		return fsaHandle;

	if ((ret = IOSUHAX_FSA_OpenDir(fsaHandle, "/dev", &folderHandle)) >= 0) {
		directoryEntry_s entry;

		while (IOSUHAX_FSA_ReadDir(fsaHandle, folderHandle, &entry) == 0) {
			if (isStringInArray(entry.name, m_whitelistedDevices, sizeof(m_whitelistedDevices) / sizeof(m_whitelistedDevices[0])))
				devices->emplace_back(std::string(entry.name));
		}
	} else {
		IOSUHAX_FSA_Close(fsaHandle);
		return ret;
	}

	IOSUHAX_FSA_Close(fsaHandle);
	return 0;
}

int PhysicalDeviceUtils::getDeviceDelta(std::vector<std::string> mountedDevices, std::vector<std::string> blacklisted, std::vector<std::string>* addedDevices, std::vector<std::string>* removedDevices, std::vector<std::string>* removedBlacklistedDevices) {
	std::vector<std::string> allMountableDevices, mountableDevices;
	int ret;

	if ((ret = getMountableDevices(&allMountableDevices)) < 0)
		return ret;

	// Need to sort vectors for set_difference to work properly
	std::sort(mountedDevices.begin(), mountedDevices.end());
	std::sort(allMountableDevices.begin(), allMountableDevices.end());
	std::sort(blacklisted.begin(), blacklisted.end());

	// Remove blacklisted devices from mountable devices
	std::set_difference(allMountableDevices.begin(), allMountableDevices.end(), blacklisted.begin(), blacklisted.end(), std::inserter(mountableDevices, mountableDevices.begin()));

	// Check differences between mounted and available devices
	std::set_difference(mountableDevices.begin(), mountableDevices.end(), mountedDevices.begin(), mountedDevices.end(), std::inserter(*addedDevices, addedDevices->begin()));
	std::set_difference(mountedDevices.begin(), mountedDevices.end(), mountableDevices.begin(), mountableDevices.end(), std::inserter(*removedDevices, removedDevices->begin()));

	// Check if any blacklisted device has been removed
    std::set_difference(blacklisted.begin(), blacklisted.end(), allMountableDevices.begin(), allMountableDevices.end(), std::inserter(*removedBlacklistedDevices, removedBlacklistedDevices->begin()));

	return 0;
}

// TODO: Replace this with a string array
bool PhysicalDeviceUtils::isStringInArray(const char* string, const char* array[], int arraySize) {
	for (int i = 0; i < arraySize; i++) {
		if (strncmp(string, array[i], strlen(array[i])) == 0)
			return true;
	}

	return false;
}

bool PhysicalDeviceUtils::isNative(const std::string& id) {
    // TODO: Add more native devices
    return (startsWith(id, "odd")
            || startsWith(id, "mlc"));
}

std::string PhysicalDeviceUtils::getNativeDeviceName(const std::string& id) {
    if (id == "odd01")
        return "Disc tickets";
    else if (id == "odd02")
        return "Disc update";
    else if (id == "odd03" || id == "odd04")
        return "Disc content";

    return id;
}

void PhysicalDeviceUtils::shrinkFATLabel(char *label) {
    unsigned i = 11;
    while (--i > 0) {
        if (label[i] == 0x20)
            label[i] = 0x00;
        else
            break;
    }
}

bool PhysicalDeviceUtils::startsWith(const std::string &sourceString, const std::string &startString) {
    return startString.length() <= sourceString.length()
           && equal(startString.begin(), startString.end(), sourceString.begin());
}
