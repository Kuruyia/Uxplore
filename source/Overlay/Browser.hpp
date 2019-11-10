#ifndef BROWSER_HPP
#define BROWSER_HPP

#include <TweenEngine/TweenManager.h>

#include "../BrowseSession.hpp"
#include "../Device/PhysicalDevice/PhyDeviceManager.hpp"
#include "Overlay.hpp"

class Browser: public Overlay {
public:
	Browser();
	
	void update(float delta) override;
	void processEvent(SDL_Event event) override;
	void render(RenderKit* renderKit) override;

private:
	BrowseSession m_browseSession;
	PhysicalDeviceManager m_physicalDeviceManager;

	TweenEngine::TweenManager m_tweenManager;
};

#endif // BROWSER_HPP