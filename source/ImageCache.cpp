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

#include <SDL2/SDL_image.h>
#include "ImageCache.h"

ImageCache::ImageCache() {

}

ImageCache &ImageCache::getInstance() {
	static ImageCache cache;
	return cache;
}

void ImageCache::setRenderer(SDL_Renderer* renderer) {
	getInstance().m_renderer = renderer;
}

SDL_Texture* ImageCache::getImage(const std::string& path) {
	static ImageCache cache = getInstance();

	auto iconIt = cache.m_textureList.find(path);
	SDL_Texture* icon;
	if (iconIt == cache.m_textureList.end()) {
		SDL_Surface* iconSurface = IMG_Load(path.c_str());

		icon = SDL_CreateTextureFromSurface(cache.m_renderer, iconSurface);
		SDL_FreeSurface(iconSurface);

		//cache.m_textureList.insert(std::make_pair(path, icon));
	} else {
		icon = iconIt->second;
	}

	return icon;
}