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

#ifndef TEXT_HPP
#define TEXT_HPP

#include <string>

#include "Drawable.h"

class Text: public Drawable {
public:
	Text(Drawable* parent, SDL_Point position, std::string text);
	~Text();

	void draw(SDL_Renderer* renderer, TTF_Font* font) override;

	std::string getText();
	void setText(std::string text);

	void setScale(float scale);

	void setColor(SDL_Color color);
	SDL_Color getColor();

private:
	std::string m_text;
	SDL_Color m_textColor;

	bool m_needsUpdate;

	SDL_Texture* m_textTexture;
	SDL_Rect m_textRect;
};

#endif // TEXT_HPP