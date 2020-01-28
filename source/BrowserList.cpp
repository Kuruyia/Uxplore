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

#include <TweenEngine/Tween.h>
#include "BrowserList.h"

BrowserList::BrowserList()
        : m_selectedItemIndex(0)
        , m_firstRow(0)
        , m_emptyListText(this, {0, 0}, "There is nothing to see here.")
{

}

void BrowserList::draw(SDL_Renderer *renderer, TTF_Font *font)
{
    for (auto &m_item : m_items)
        m_item->draw(renderer, font);

    //m_emptyListText.draw(renderer, font);
}

void BrowserList::update(float delta)
{
    m_tweenManager.update(delta);
}

void BrowserList::addItem(const Entry &entry)
{
    SDL_Point itemPosition = getPositionFromIndex(m_items.size());

    std::shared_ptr<BrowserItem> item(new BrowserItem(this, entry, itemPosition));

    if (m_items.empty())
        item->setSelected(true);

    m_items.emplace_back(std::move(item));
}

std::shared_ptr<BrowserItem> BrowserList::getItem(unsigned int index)
{
    return m_items.at(index);
}

SDL_Point BrowserList::getPositionFromIndex(unsigned int index)
{
    SDL_Point position;
    position.x = ITEM_OFFSET + (ITEM_WIDTH + ITEM_MARGIN_X) * (m_items.size() % ITEM_PER_ROW);
    position.y = ITEM_OFFSET + (ITEM_HEIGHT + ITEM_MARGIN_Y) * int(m_items.size() / ITEM_PER_ROW);

    return position;
}

std::shared_ptr<BrowserItem> BrowserList::getSelectedItem()
{
    return getItem(m_selectedItemIndex);
}

unsigned int BrowserList::getSelectedItemIndex()
{
    return m_selectedItemIndex;
}

void BrowserList::setSelectedItemIndex(unsigned int index)
{
    if (index < 0)
        index = 0;
    else if (index >= m_items.size())
        index = m_items.size() - 1;

    m_items.at(m_selectedItemIndex)->setSelected(false);
    m_selectedItemIndex = index;
    m_items.at(m_selectedItemIndex)->setSelected(true);

    int firstRow = m_firstRow;
    int newRow = int(m_selectedItemIndex / ITEM_PER_ROW);

    if (newRow < m_firstRow)
        m_firstRow = newRow;
    else if (newRow >= m_firstRow + VISIBLE_ROWS)
        m_firstRow = newRow - VISIBLE_ROWS + 1;

    if (m_firstRow != firstRow) {
        TweenEngine::Tween::to(*this, TweenDrawable::POSITION_Y, 0.2f)
                .target(-(m_firstRow * ITEM_HEIGHT + m_firstRow * ITEM_MARGIN_Y))
                .start(m_tweenManager);
    }
}

void BrowserList::moveSelectedItemIndex(int indexDelta)
{
    const int newIndex = (int) m_selectedItemIndex + indexDelta;
    setSelectedItemIndex(newIndex > 0 ? newIndex : 0);
}

void BrowserList::reset()
{
    m_selectedItemIndex = 0;
    m_firstRow = 0;

    m_items.erase(m_items.begin(), m_items.end());
}