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

#ifndef RENDERKIT_HPP
#define RENDERKIT_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class RenderKit {
public:
    RenderKit(SDL_Renderer *primaryRenderer, SDL_Renderer *secondaryRenderer, TTF_Font *textFont);

    SDL_Renderer *getPrimaryRenderer();
    SDL_Renderer *getSecondaryRenderer();
    TTF_Font *getTextFont();

private:
    SDL_Renderer *m_primaryRenderer;
    SDL_Renderer *m_secondaryRenderer;

    TTF_Font *m_textFont;
};

#endif // RENDERKIT_HPP