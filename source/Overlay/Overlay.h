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

#ifndef OVERLAY_HPP
#define OVERLAY_HPP


#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>

// Remove altivec vector definition
#ifdef vector
#undef vector
#endif

class Overlay
{
public:
    virtual ~Overlay() = default;

    virtual void processEvent(const SDL_Event& event) = 0;

    virtual void update(const float& delta) = 0;

    virtual void renderPrimary(SDL_Renderer& renderer, TTF_Font& font) = 0;
    virtual void renderSecondary(SDL_Renderer &renderer, TTF_Font &font) = 0;
};

#endif // OVERLAY_HPP