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

#include "RenderKit.h"

RenderKit::RenderKit(SDL_Renderer *primaryRenderer, SDL_Renderer *secondaryRenderer, TTF_Font *textFont)
        : m_primaryRenderer(primaryRenderer)
        , m_secondaryRenderer(secondaryRenderer)
        , m_textFont(textFont)
{

}

SDL_Renderer *RenderKit::getPrimaryRenderer()
{
    return m_primaryRenderer;
}

SDL_Renderer *RenderKit::getSecondaryRenderer()
{
    return m_secondaryRenderer;
}

TTF_Font *RenderKit::getTextFont()
{
    return m_textFont;
}