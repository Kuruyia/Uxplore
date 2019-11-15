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

#include "BrowseSession.hpp"

#include <Entry/EntryPhysicalMountedPartition.hpp>

BrowseSession::BrowseSession(PhysicalDeviceManager *physicalDeviceManager)
: m_physicalDeviceManager(physicalDeviceManager)
{
	reloadList();
}

void BrowseSession::draw(SDL_Renderer* renderer, TTF_Font* font) {
	m_browserList.draw(renderer, font);
}

void BrowseSession::update(float delta) {
	m_browserList.update(delta);
	m_tweenManager.update(delta);
}

void BrowseSession::processEvent(SDL_Event event) {
	switch (event.type) {
		case SDL_JOYBUTTONDOWN: {
			switch (event.jbutton.button) {
				case 13: {
					m_browserList.moveSelectedItemIndex(-ITEM_PER_ROW);

					break;
				}
				case 15: {
					m_browserList.moveSelectedItemIndex(ITEM_PER_ROW);

					break;
				}
				case 14: {
					m_browserList.moveSelectedItemIndex(1);

					break;
				}
				case 12: {
					m_browserList.moveSelectedItemIndex(-1);

					break;
				}
				case 6: {
					m_navigationHistory.back();

					break;
				}
				case 7: {
					m_navigationHistory.forward();

					break;
				}
				case 2: {
				    reloadList();

				    break;
				}
			}

			break;
		}
	}
}

void BrowseSession::reloadList() {
    m_browserList.reset();

	if (m_navigationHistory.getHistoryPosition() == 0) {
        for (const auto &device : m_physicalDeviceManager->getInsertedDevices()) {
            for (const auto &partition : device->getMountedPartitions()) {
                m_browserList.addItem(EntryPhysicalMountedPartition(device, partition));
            }
        }
	} else {
		
	}
}

BrowserList* BrowseSession::getBrowserList() {
	return &m_browserList;
}

void BrowseSession::notifyDevicesChanged() {
	if (m_navigationHistory.getHistoryPosition() == 0)
		reloadList();
}