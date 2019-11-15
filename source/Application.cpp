#include "Application.hpp"
#include "ImageCache.hpp"
#include "Overlay/Browser.hpp"
#include <whb/log.h>
#include <iosuhax.h>

Application::Application() {
	for (int i = 0; i < SDL_NumJoysticks(); i++)
    	m_openedJoysticks.emplace_back(SDL_JoystickOpen(i));

    m_sdlWindowTV = SDL_CreateWindow(nullptr, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_WIIU_TV_ONLY);
    m_sdlRendererTV = SDL_CreateRenderer(m_sdlWindowTV, -1, SDL_RENDERER_ACCELERATED);

    m_sdlWindowGamepad = SDL_CreateWindow(nullptr, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_WIIU_GAMEPAD_ONLY);
    m_sdlRendererGamepad = SDL_CreateRenderer(m_sdlWindowGamepad, -1, SDL_RENDERER_ACCELERATED);

    m_textFont = TTF_OpenFont("romfs:/res/fonts/opensans.ttf", 32);
    m_renderKit = new RenderKit(m_sdlRendererTV, m_sdlRendererGamepad, m_textFont);
    ImageCache::getInstance().setRenderer(m_sdlRendererTV);

    std::unique_ptr<Browser> browser(new Browser());
    m_overlays.emplace_back(std::move(browser));
}

Application::~Application() {
	SDL_DestroyRenderer(m_sdlRendererTV);
    SDL_DestroyWindow(m_sdlWindowTV);

    SDL_DestroyRenderer(m_sdlRendererGamepad);
    SDL_DestroyWindow(m_sdlWindowGamepad);

    TTF_CloseFont(m_textFont);

    for (auto currentJoystick : m_openedJoysticks) {
    		if (SDL_JoystickGetAttached(currentJoystick))
    		    SDL_JoystickClose(currentJoystick);
    }
}

void Application::render(float delta) {
	SDL_SetRenderDrawBlendMode(m_sdlRendererTV, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(m_sdlRendererTV, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(m_sdlRendererTV);

	SDL_SetRenderDrawBlendMode(m_sdlRendererGamepad, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(m_sdlRendererGamepad, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(m_sdlRendererGamepad);

	for (auto & m_overlay : m_overlays) {
		Overlay* currentOverlay = m_overlay.get();

		currentOverlay->update(delta);
		currentOverlay->render(m_renderKit);
	}

	SDL_Event event;
	if (SDL_PollEvent(&event) && !m_overlays.empty()) {
		m_overlays.back()->processEvent(event);
	}

	SDL_RenderPresent(m_sdlRendererTV);
	SDL_RenderPresent(m_sdlRendererGamepad);
}