#ifndef BROWSESESSION_HPP
#define BROWSESESSION_HPP

#include <map>
#include <memory>
#include <vector>
#include <SDL2/SDL_image.h>
#include <TweenEngine/TweenManager.h>
#include <Device/PhysicalDevice/PhyDeviceManager.hpp>

#include "GUI/BrowserItem.hpp"
#include "BrowserList.hpp"
#include "NavigationHistory.hpp"
#include "TweenObjects.hpp"

class BrowseSession: public TweenDrawable {
public:
	BrowseSession(PhysicalDeviceManager *physicalDeviceManager);

	void draw(SDL_Renderer* renderer, TTF_Font* font) override;
	void update(float delta);
	void processEvent(SDL_Event event);

	void reloadList();
	BrowserList* getBrowserList();

	void notifyDevicesChanged();

private:
	BrowserList m_browserList;
	NavigationHistory m_navigationHistory;

	PhysicalDeviceManager *m_physicalDeviceManager;

	TweenEngine::TweenManager m_tweenManager;
};

#endif // BROWSESESSION_HPP