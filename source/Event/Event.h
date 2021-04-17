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

#ifndef UXPLORE_CPP_EVENT_H
#define UXPLORE_CPP_EVENT_H

enum class EventType
{
    EVENT_SDL,
    EVENT_PHYSICAL_DEVICES_CHANGED
};

union EventData
{
    SDL_Event m_sdlEvent;
};

struct Event
{
    EventType m_eventType;
    EventData m_eventData;
};

#endif //UXPLORE_CPP_EVENT_H
