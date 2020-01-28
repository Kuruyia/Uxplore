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

#ifndef BROWSERLIST_HPP
#define BROWSERLIST_HPP

#define VISIBLE_ROWS 6
#define ITEM_PER_ROW 3
#define ITEM_WIDTH 396
#define ITEM_HEIGHT 89
#define ITEM_MARGIN_X 23
#define ITEM_MARGIN_Y 18
#define ITEM_OFFSET 23

#include <map>
#include <memory>
#include <vector>
#include <SDL2/SDL_image.h>
#include <TweenEngine/TweenManager.h>

#include "Entry/Entry.h"
#include "TweenObjects.h"
#include "GUI/BrowserItem.h"

class BrowserList : public TweenDrawable {
public:
    BrowserList();

    void draw(SDL_Renderer &renderer, TTF_Font &font) override;

    void update(const float &delta);

    void addItem(const Entry &entry);
    std::shared_ptr<BrowserItem> getItem(const size_t &index);
    void reset();

    std::shared_ptr<BrowserItem> getSelectedItem();
    unsigned int getSelectedItemIndex();
    void setSelectedItemIndex(size_t index);
    void moveSelectedItemIndex(const int &indexDelta);

private:
    SDL_Point getPositionFromIndex(unsigned int index);

    std::vector<std::shared_ptr<BrowserItem>> m_items;
    Text m_emptyListText;
    size_t m_selectedItemIndex;
    int m_firstRow;

    std::map<Entry::EntryType, SDL_Texture *> m_icons;
    TweenEngine::TweenManager m_tweenManager;
};

#endif // BROWSERLIST_HPP