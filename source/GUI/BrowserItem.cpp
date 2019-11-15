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

#include "BrowserItem.hpp"
#include "../ImageCache.hpp"

BrowserItem::BrowserItem(Drawable* parent, const Entry &entry, SDL_Point pos)
: m_selected(false)
, m_entry(entry.clone())
, m_entryName(this, {96, 0}, m_entry->getText())
{
    m_icon = ImageCache::getImage("romfs:/res/icons/" + m_entry->getIconName());

	m_entryName.setScale(0.7f);

	setParent(parent);
	setRelativePosition(pos);
}

void BrowserItem::draw(SDL_Renderer* renderer, TTF_Font* font) {
	SDL_Point position = getAbsolutePosition();
	SDL_Rect rect;

	rect.x = position.x;
	rect.y = position.y;
	rect.w = 396;
	rect.h = 90;

	SDL_SetRenderDrawColor(renderer, 41, 128, 185, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawRect(renderer, &rect);
	if (m_selected) {
		rect.x += 90;
		rect.w -= 90;
		rect.h--;

		SDL_RenderFillRect(renderer, &rect);

		rect.x -= 90;
		rect.w += 90;
		rect.h++;
	}
	
	SDL_SetRenderDrawColor(renderer, 149, 165, 166, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawLine(renderer, position.x + 90, position.y, position.x + 90, position.y + 89);

	rect.x += 13;
	rect.y += 13;
	rect.w = 64;
	rect.h = 64;
	SDL_RenderCopy(renderer, m_icon, NULL, &rect);

	m_entryName.draw(renderer, font);
}

bool BrowserItem::isSelected() {
	return m_selected;
}

void BrowserItem::setSelected(bool selected) {
	m_selected = selected;
}