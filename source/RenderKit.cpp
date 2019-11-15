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

#include "RenderKit.hpp"

RenderKit::RenderKit(SDL_Renderer* tvRenderer, SDL_Renderer* gamepadRenderer, TTF_Font* textFont)
: m_tvRenderer(tvRenderer)
, m_gamepadRenderer(gamepadRenderer)
, m_textFont(textFont)
{

}

SDL_Renderer* RenderKit::getTVRenderer() {
	return m_tvRenderer;
}

SDL_Renderer* RenderKit::getGamepadRenderer() {
	return m_gamepadRenderer;
}

TTF_Font* RenderKit::getTextFont() {
	return m_textFont;
}