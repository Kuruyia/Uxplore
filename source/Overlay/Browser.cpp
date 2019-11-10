#include "Browser.hpp"

#include <TweenEngine/Tween.h>
#include <whb/log.h>
#include <fat.h>

#include "../Device/PhysicalDevice/PhyDeviceUtils.hpp"
#include "../DiscInterface/DiscInterface.hpp"
#include "../Entry/EntryFile.hpp"
#include "../BrowserList.hpp"
#include "../File.hpp"

Browser::Browser()
: m_browseSession(&m_physicalDeviceManager)
{
	/*DIR *pdir;
    struct dirent *pent;
    
    pdir=opendir("usb001:/");
    
    WHBLogPrint("dir opened");
    
    if (pdir) {
        while ((pent=readdir(pdir))!=NULL) {
            if(strcmp(".", pent->d_name) == 0 || strcmp("..", pent->d_name) == 0)
                continue;
            if(pent->d_type == DT_DIR)
                WHBLogPrintf("[%s]", pent->d_name);
            else
                WHBLogPrintf("%s", pent->d_name);
        }
        closedir(pdir);
    } else {
        WHBLogPrintf("opendir() failure; terminating");
    }*/
}

void Browser::update(float delta) {
	m_tweenManager.update(delta);

	if (m_physicalDeviceManager.update())
		m_browseSession.notifyDevicesChanged();
	
	m_browseSession.update(delta);
}

void Browser::processEvent(SDL_Event event) {
	m_browseSession.processEvent(event);
}

void Browser::render(RenderKit* renderKit) {
	SDL_Renderer* tvRenderer = renderKit->getTVRenderer();
	SDL_Renderer* gamepadRenderer = renderKit->getGamepadRenderer();
	TTF_Font* font = renderKit->getTextFont();

	// Render TV
	m_browseSession.draw(tvRenderer, font);

	// Render Gamepad
	SDL_SetRenderDrawColor(gamepadRenderer, 255, 0, 0, 128);
	SDL_Rect rect;
	rect.x = 50;
	rect.y = 50;
	rect.w = 200;
	rect.h = 200;
	SDL_RenderFillRect(gamepadRenderer, &rect);
	rect.x += 100;
	SDL_SetRenderDrawColor(gamepadRenderer, 0, 255, 0, 128);
	SDL_RenderFillRect(gamepadRenderer, &rect);
	rect.x -= 50;
	rect.y += 100;
	SDL_SetRenderDrawColor(gamepadRenderer, 0, 0, 255, 128);
	SDL_RenderFillRect(gamepadRenderer, &rect);
}