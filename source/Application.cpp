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

#include "Application.h"
#include "ImageCache.h"
#include "Overlay/Browser.h"

Application::Application()
        : m_lastPhysicalDeviceManagerUpdate(0)
{
    for (int i = 0; i < SDL_NumJoysticks(); i++)
        m_openedJoysticks.emplace_back(SDL_JoystickOpen(i));

    m_sdlWindowTV = SDL_CreateWindow(nullptr, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720,
                                     SDL_WINDOW_WIIU_TV_ONLY);
    m_sdlRendererTV = SDL_CreateRenderer(m_sdlWindowTV, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawBlendMode(m_sdlRendererTV, SDL_BLENDMODE_BLEND);

    m_sdlWindowGamepad = SDL_CreateWindow(nullptr, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720,
                                          SDL_WINDOW_WIIU_GAMEPAD_ONLY);
    m_sdlRendererGamepad = SDL_CreateRenderer(m_sdlWindowGamepad, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawBlendMode(m_sdlRendererGamepad, SDL_BLENDMODE_BLEND);

    m_textFont = TTF_OpenFont("romfs:/res/fonts/opensans.ttf", 32);
    ImageCache::getInstance().setRenderer(m_sdlRendererTV);

    m_overlayManager.pushOverlay<Browser>(m_physicalDeviceManager);
}

Application::~Application()
{
    SDL_DestroyRenderer(m_sdlRendererTV);
    SDL_DestroyWindow(m_sdlWindowTV);

    SDL_DestroyRenderer(m_sdlRendererGamepad);
    SDL_DestroyWindow(m_sdlWindowGamepad);

    TTF_CloseFont(m_textFont);

    for (auto currentJoystick : m_openedJoysticks) {
        if (SDL_JoystickGetAttached(currentJoystick))
            SDL_JoystickClose(currentJoystick);
    }
}

void Application::render(const float &delta)
{
    // Poll any SDL sdlEvent and dispatch it to the topmost overlay
    Event sdlEvent = {
            EventType::EVENT_SDL,
            {
                    .m_sdlEvent = SDL_Event()
            }
    };

    if (SDL_PollEvent(&sdlEvent.m_eventData.m_sdlEvent))
    {
        m_overlayManager.dispatchEventToTopmost(sdlEvent);
    }

    // Update the physical device manager every 2s
    if (SDL_TICKS_PASSED(SDL_GetTicks(), m_lastPhysicalDeviceManagerUpdate + 2000))
    {
        m_lastPhysicalDeviceManagerUpdate = SDL_GetTicks();

        if (m_physicalDeviceManager.update())
        {
            m_overlayManager.dispatchEvent({
                                                   EventType::EVENT_PHYSICAL_DEVICES_CHANGED,
                                                   {}
                                           });
        }
    }

    // Update every overlay
    m_overlayManager.update(delta);

    // Clear and draw the TV for every overlay
    SDL_SetRenderDrawColor(m_sdlRendererTV, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(m_sdlRendererTV);
    m_overlayManager.renderPrimary(*m_sdlRendererTV, *m_textFont);

    // Clear and draw the DRC for every overlay
    SDL_SetRenderDrawColor(m_sdlRendererGamepad, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(m_sdlRendererGamepad);
    m_overlayManager.renderSecondary(*m_sdlRendererGamepad, *m_textFont);

    SDL_RenderPresent(m_sdlRendererTV);
    SDL_RenderPresent(m_sdlRendererGamepad);
}