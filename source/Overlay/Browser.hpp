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