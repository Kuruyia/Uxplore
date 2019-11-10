#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <memory>
#include <vector>

#include "RenderKit.hpp"
#include "Overlay/Overlay.hpp"

class Application {
public:
	Application();
	~Application();

	void render(float delta);

private:
	SDL_Window* m_sdlWindowTV;
	SDL_Renderer* m_sdlRendererTV;

	SDL_Window* m_sdlWindowGamepad;
	SDL_Renderer* m_sdlRendererGamepad;

	TTF_Font* m_textFont;
	RenderKit* m_renderKit;

	std::vector<SDL_Joystick*> m_openedJoysticks;

	std::vector<std::unique_ptr<Overlay>> m_overlays;
};

#endif // APPLICATION_HPP