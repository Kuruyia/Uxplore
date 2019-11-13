#ifndef PHYDEVICEUTILS_HPP
#define PHYDEVICEUTILS_HPP

#include <string>
#include <vector>

class PhysicalDeviceUtils {
public:
	static int getMountableDevices(std::vector<std::string>* devices);
	static int getDeviceDelta(std::vector<std::string> mountedDevices, std::vector<std::string> *addedDevices,
                              std::vector<std::string> *removedDevices);

    static bool isNative(const std::string &id);
    static std::string getNativeDeviceName(const std::string& id);
    static void shrinkFATLabel(char* label);

private:
    static bool isStringInArray(const char* string, const char* array[], int arraySize);
    static bool startsWith(const std::string& sourceString, const std::string& startString);
};

#endif // PHYDEVICEUTILS_HPP