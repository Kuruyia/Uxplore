/*
    Uxplore
    Copyright (C) 2019-2019, Kuruyia

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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