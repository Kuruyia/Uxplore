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

#ifndef PHYDEVICEUTILS_HPP
#define PHYDEVICEUTILS_HPP

#include <string>
#include <vector>

class PhysicalDeviceUtils {
public:
    static int getMountableDevices(std::vector<std::string> &devices);
    static int getDeviceDelta(std::vector<std::string> mountedDevices, std::vector<std::string> &addedDevices,
                              std::vector<std::string> &removedDevices);

    static bool isNative(const std::string &id);
    static std::string getNativeDeviceName(const std::string &id);

    static void shrinkFATLabel(char *label);

private:
    static bool isStringInArray(const char *string, const char *array[], int arraySize);

    static bool startsWith(const std::string &sourceString, const std::string &startString);
};

#endif // PHYDEVICEUTILS_HPP