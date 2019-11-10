#include <SDL2/SDL_image.h>
#include "ImageCache.hpp"

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