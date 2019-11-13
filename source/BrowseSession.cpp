#include "BrowseSession.hpp"

#include <Entry/EntryPhyDevice.hpp>

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
        for (const auto & mountedDevice : m_physicalDeviceManager->getInsertedDevices()) {
            m_browserList.addItem(new EntryPhyDevice(mountedDevice.get()));
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