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

#include "Browser.h"

#include "../Macros.h"
#include "OverlayManager.hpp"

Browser::Browser(OverlayManager &manager)
        : ManagedOverlay(manager)
        , m_browseSession(m_physicalDeviceManager)
{

}

void Browser::update(const float &delta)
{
    m_tweenManager.update(delta);

    if (m_physicalDeviceManager.update())
        m_browseSession.notifyDevicesChanged();

    m_browseSession.update(delta);
}

void Browser::processEvent(const SDL_Event &event)
{
    m_browseSession.processEvent(event);
}

void Browser::renderPrimary(SDL_Renderer &renderer, TTF_Font &font)
{
    m_browseSession.draw(renderer, font);
}

void Browser::renderSecondary(SDL_Renderer& renderer, UNUSED_PARAM TTF_Font& font)
{
    SDL_SetRenderDrawColor(&renderer, 255, 0, 0, 128);
    SDL_Rect rect;
    rect.x = 50;
    rect.y = 50;
    rect.w = 200;
    rect.h = 200;
    SDL_RenderFillRect(&renderer, &rect);
    rect.x += 100;
    SDL_SetRenderDrawColor(&renderer, 0, 255, 0, 128);
    SDL_RenderFillRect(&renderer, &rect);
    rect.x -= 50;
    rect.y += 100;
    SDL_SetRenderDrawColor(&renderer, 0, 0, 255, 128);
    SDL_RenderFillRect(&renderer, &rect);
}
