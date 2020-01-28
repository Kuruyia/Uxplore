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

#ifndef IMAGECACHE_HPP
#define IMAGECACHE_HPP

#include <SDL2/SDL.h>
#include <memory>
#include <map>

class ImageCache {
public:
    static ImageCache &getInstance();

    static void setRenderer(SDL_Renderer *renderer);

    static SDL_Texture *getImage(const std::string &path);

private:
    ImageCache();

    SDL_Renderer *m_renderer;
    std::map<std::string, SDL_Texture *> m_textureList;
};

#endif // IMAGECACHE_HPP