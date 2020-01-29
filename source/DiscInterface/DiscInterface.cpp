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

#include "DiscInterface.h"
#include "DiscInterfaceWrapper.h"

#include <iosuhax.h>

#include <utility>

DiscInterface::DiscInterface(std::string devicePath)
        : m_fsaFdUSB(-1)
        , m_usbFd(-1)
        , m_devicePath(std::move(devicePath))
{
    m_discInterface = (DISC_INTERFACE) {
            DEVICE_TYPE_WII_U_USB,
            FEATURE_MEDIUM_CANREAD | FEATURE_MEDIUM_CANWRITE | FEATURE_WII_U_USB,
            (FN_MEDIUM_STARTUP) &DiscInterfaceWrapper::startup,
            (FN_MEDIUM_ISINSERTED) &DiscInterfaceWrapper::isInserted,
            (FN_MEDIUM_READSECTORS) &DiscInterfaceWrapper::readSectors,
            (FN_MEDIUM_WRITESECTORS) &DiscInterfaceWrapper::writeSectors,
            (FN_MEDIUM_CLEARSTATUS) &DiscInterfaceWrapper::clearStatus,
            (FN_MEDIUM_SHUTDOWN) &DiscInterfaceWrapper::shutdown,
            this
    };
}

DISC_INTERFACE *DiscInterface::getInterface()
{
    return &m_discInterface;
}

// Adapted code from: https://github.com/dimok789/libiosuhax/blob/master/source/iosuhax_disc_interface.c
bool DiscInterface::startup()
{
    if (!openFSA())
        return false;

    if (m_usbFd < 0) {
        int res = IOSUHAX_FSA_RawOpen(m_fsaFdUSB, m_devicePath.c_str(), &m_usbFd);
        if (res < 0) {
            closeFSA();
            m_usbFd = -1;
        }
    }
    return (m_usbFd >= 0);
}

bool DiscInterface::isInserted() const
{
    return (m_fsaFdUSB >= 0) && (m_usbFd >= 0);
}

bool DiscInterface::clearStatus()
{
    return true;
}

bool DiscInterface::shutdown()
{
    if (!isInserted())
        return false;

    IOSUHAX_FSA_RawClose(m_fsaFdUSB, m_usbFd);
    closeFSA();
    m_usbFd = -1;
    return true;
}

bool DiscInterface::readSectors(uint32_t sector, uint32_t numSectors, void *buffer) const
{
    if (!isInserted())
        return false;

    int res = IOSUHAX_FSA_RawRead(m_fsaFdUSB, buffer, 512, numSectors, sector, m_usbFd);
    if (res < 0) {
        return false;
    }

    return true;
}

bool DiscInterface::writeSectors(uint32_t sector, uint32_t numSectors, const void *buffer)
{
    if (!isInserted())
        return false;

    int res = IOSUHAX_FSA_RawWrite(m_fsaFdUSB, buffer, 512, numSectors, sector, m_usbFd);
    if (res < 0) {
        return false;
    }

    return true;
}

bool DiscInterface::openFSA()
{
    if (m_fsaFdUSB < 0)
        m_fsaFdUSB = IOSUHAX_FSA_Open();

    return m_fsaFdUSB >= 0;
}

void DiscInterface::closeFSA()
{
    if (m_fsaFdUSB >= 0)
        IOSUHAX_FSA_Close(m_fsaFdUSB);
}