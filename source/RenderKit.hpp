#ifndef RENDERKIT_HPP
#define RENDERKIT_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class RenderKit {
public:
	RenderKit(SDL_Renderer* tvRenderer, SDL_Renderer* gamepadRenderer, TTF_Font* textFont);

	SDL_Renderer* getTVRenderer();
	SDL_Renderer* getGamepadRenderer();

	TTF_Font* getTextFont();

private:
	SDL_Renderer* m_tvRenderer;
	SDL_Renderer* m_gamepadRenderer;

	TTF_Font* m_textFont;
};

#endif // RENDERKIT_HPP