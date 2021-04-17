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

#ifndef DISCINTERFACE_HPP
#define DISCINTERFACE_HPP

#include <string>
#include <iosuhax_disc_interface.h>

class DiscInterface {
public:
    explicit DiscInterface(std::string devicePath);

    DISC_INTERFACE *getInterface();

    bool startup();
    bool isInserted() const;
    bool clearStatus();
    bool shutdown();
    bool readSectors(uint32_t sector, uint32_t numSectors, void *buffer) const;
    bool writeSectors(uint32_t sector, uint32_t numSectors, const void *buffer);

private:
    bool openFSA();
    void closeFSA();

    int m_fsaFdUSB;
    int m_usbFd;

    std::string m_devicePath;
    DISC_INTERFACE m_discInterface{};
};

#endif // DISCINTERFACE_HPP