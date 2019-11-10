#ifndef IMAGECACHE_HPP
#define IMAGECACHE_HPP

#include <SDL2/SDL.h>
#include <memory>
#include <map>

class ImageCache {
public:
	static ImageCache& getInstance();

	static void setRenderer(SDL_Renderer* renderer);

	static SDL_Texture* getImage(const std::string& path);

private:
	ImageCache();

	SDL_Renderer* m_renderer;
	std::map<std::string, SDL_Texture*> m_textureList;
};

#endif // IMAGECACHE_HPP