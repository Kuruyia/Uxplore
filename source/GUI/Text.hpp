#ifndef TEXT_HPP
#define TEXT_HPP

#include <string>

#include "Drawable.hpp"

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