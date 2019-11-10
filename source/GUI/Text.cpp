#include "Text.hpp"

Text::Text(Drawable* parent, SDL_Point position, std::string text)
: m_text(text)
, m_needsUpdate(true)
, m_textColor({0, 0, 0})
{
	setParent(parent);
	setRelativePosition(position);
}

Text::~Text() {
	SDL_DestroyTexture(m_textTexture);
}

void Text::draw(SDL_Renderer* renderer, TTF_Font* font) {
	if (m_needsUpdate) {
		SDL_Surface* textSurface = TTF_RenderUTF8_Blended(font, m_text.c_str(), m_textColor);
		m_textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

		m_textRect.w = textSurface->w * getScale();
		m_textRect.h = textSurface->h * getScale();

		SDL_FreeSurface(textSurface);

		m_needsUpdate = false;
	}

	SDL_Point position = getAbsolutePosition();
	m_textRect.x = position.x;
	m_textRect.y = position.y;

	SDL_RenderCopy(renderer, m_textTexture, nullptr, &m_textRect);
}

std::string Text::getText() {
	return m_text;
}

void Text::setText(std::string text) {
	m_text = std::move(text);
    m_needsUpdate = true;
}

void Text::setScale(float scale) {
	m_needsUpdate = true;
	Drawable::setScale(scale);
}

void Text::setColor(SDL_Color color) {
    m_textColor = color;
    m_needsUpdate = true;
}

SDL_Color Text::getColor() {
    return m_textColor;
}



