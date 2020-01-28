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

#include "DiscInterfaceWrapper.h"

bool DiscInterfaceWrapper::startup(void *data)
{
    return getInterfaceFromData(data)->startup();
}

bool DiscInterfaceWrapper::isInserted(void *data)
{
    return getInterfaceFromData(data)->isInserted();
}

bool DiscInterfaceWrapper::clearStatus(void *data)
{
    return getInterfaceFromData(data)->clearStatus();
}

bool DiscInterfaceWrapper::shutdown(void *data)
{
    return getInterfaceFromData(data)->shutdown();
}

bool DiscInterfaceWrapper::readSectors(void *data, uint32_t sector, uint32_t numSectors, void *buffer)
{
    return getInterfaceFromData(data)->readSectors(sector, numSectors, buffer);
}

bool DiscInterfaceWrapper::writeSectors(void *data, uint32_t sector, uint32_t numSectors, const void *buffer)
{
    return getInterfaceFromData(data)->writeSectors(sector, numSectors, buffer);
}

DiscInterface *DiscInterfaceWrapper::getInterfaceFromData(void *data)
{
    return static_cast<DiscInterface *>(data);
}