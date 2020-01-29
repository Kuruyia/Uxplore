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

#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <memory>
#include <vector>

#include "Overlay/OverlayManager.hpp"

class Application {
public:
    Application();
    ~Application();

    void render(const float &delta);

private:
    SDL_Window *m_sdlWindowTV;
    SDL_Renderer *m_sdlRendererTV;

    SDL_Window *m_sdlWindowGamepad;
    SDL_Renderer *m_sdlRendererGamepad;

    TTF_Font *m_textFont;

    std::vector<SDL_Joystick *> m_openedJoysticks;

    OverlayManager m_overlayManager;
};

#endif // APPLICATION_HPP