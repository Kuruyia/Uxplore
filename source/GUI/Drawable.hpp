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