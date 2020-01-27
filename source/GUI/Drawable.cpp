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

#include "Drawable.h"

float Drawable::getScale() {
	return m_scale;
}

void Drawable::setScale(float scale) {
	m_scale = scale;
}

SDL_Point Drawable::getRelativePosition() {
	return m_relativePosition;
}

void Drawable::setRelativePosition(SDL_Point relativePosition) {
	m_relativePosition = relativePosition;
}

void Drawable::setRelativePosition(int positionX, int positionY) {
	SDL_Point position = {positionX, positionY};
	setRelativePosition(position);
}

SDL_Point Drawable::getAbsolutePosition() {
	SDL_Point parentPosition = {0, 0};
	SDL_Point myPosition = getRelativePosition();
	if (m_parent != NULL)
		parentPosition = m_parent->getAbsolutePosition();

	SDL_Point absolutePosition;
	absolutePosition.x = myPosition.x + parentPosition.x;
	absolutePosition.y = myPosition.y + parentPosition.y;

	return absolutePosition;
}

Drawable* Drawable::getParent() {
	return m_parent;
}

void Drawable::setParent(Drawable* parent) {
	m_parent = parent;
}