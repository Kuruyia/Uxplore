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