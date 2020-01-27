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

#ifndef DRAWABLE_HPP
#define DRAWABLE_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class Drawable {
public:
	virtual void draw(SDL_Renderer* renderer, TTF_Font* font) = 0;

	float getScale();

    virtual void setScale(float scale);

	SDL_Point getRelativePosition();
	void setRelativePosition(SDL_Point relativePosition);
	void setRelativePosition(int positionX, int positionY);

	SDL_Point getAbsolutePosition();

	Drawable* getParent();
	void setParent(Drawable* parent);

private:
	float m_scale = 1;
	SDL_Point m_relativePosition = {0, 0};

	Drawable* m_parent = nullptr;
};

#endif // DRAWABLE_HPP