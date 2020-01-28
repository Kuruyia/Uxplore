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

#include "Text.h"

Text::Text(Drawable *parent, const SDL_Point &position, const std::string &text)
        : m_text(text)
        , m_needsUpdate(true)
        , m_textColor({0, 0, 0})
{
    setParent(parent);
    setRelativePosition(position);
}

Text::~Text()
{
    SDL_DestroyTexture(m_textTexture);
}

void Text::draw(SDL_Renderer &renderer, TTF_Font &font)
{
    if (m_needsUpdate) {
        SDL_Surface *textSurface = TTF_RenderUTF8_Blended(&font, m_text.c_str(), m_textColor);
        m_textTexture = SDL_CreateTextureFromSurface(&renderer, textSurface);

        m_textRect.w = textSurface->w * getScale();
        m_textRect.h = textSurface->h * getScale();

        SDL_FreeSurface(textSurface);

        m_needsUpdate = false;
    }

    SDL_Point position = getAbsolutePosition();
    m_textRect.x = position.x;
    m_textRect.y = position.y;

    SDL_RenderCopy(&renderer, m_textTexture, nullptr, &m_textRect);
}

std::string Text::getText()
{
    return m_text;
}

void Text::setText(std::string text)
{
    m_text = std::move(text);
    m_needsUpdate = true;
}

void Text::setScale(const float &scale)
{
    m_needsUpdate = true;
    Drawable::setScale(scale);
}

void Text::setColor(SDL_Color color)
{
    m_textColor = color;
    m_needsUpdate = true;
}

SDL_Color Text::getColor()
{
    return m_textColor;
}



