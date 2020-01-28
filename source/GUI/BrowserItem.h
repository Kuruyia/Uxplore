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

#ifndef BROWSERITEM_HPP
#define BROWSERITEM_HPP

#include <string>
#include <memory>

#include "Text.h"
#include "Entry/Entry.h"
#include "TweenObjects.h"

class BrowserItem : public TweenDrawable {
public:
    BrowserItem(Drawable *parent, const Entry &entry, const SDL_Point &pos);
    ~BrowserItem() override = default;

    void draw(SDL_Renderer &renderer, TTF_Font &font) override;

    bool isSelected();
    void setSelected(bool selected);

private:
    std::unique_ptr<Entry> m_entry;
    Text m_entryName;
    SDL_Texture *m_icon;

    bool m_selected;
};

#endif // BROWSERITEM_HPP