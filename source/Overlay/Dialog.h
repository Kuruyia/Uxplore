/*
    Uxplore
    Copyright (C) 2019-2021, Kuruyia

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

#ifndef DIALOG_HPP
#define DIALOG_HPP

#include <string>
#include <TweenEngine/TweenManager.h>

#include "ManagedOverlay.h"

class Dialog : public ManagedOverlay {
public:
    explicit Dialog(OverlayManager &manager, const std::string &message);
    ~Dialog() override = default;

    void processEvent(const SDL_Event &event) override;

    void update(const float &delta) override;

    void renderPrimary(SDL_Renderer &renderer, TTF_Font &font) override;
    void renderSecondary(SDL_Renderer &renderer, TTF_Font &font) override;

private:
    std::string m_message;
};

#endif // DIALOG_HPP