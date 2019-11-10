#ifndef OVERLAY_HPP
#define OVERLAY_HPP

#include "../RenderKit.hpp"

class Overlay {
public:
	virtual ~Overlay() {};

	virtual void update(float delta) = 0;
	virtual void processEvent(SDL_Event event) = 0;
	virtual void render(RenderKit* renderKit) = 0;
};

#endif // OVERLAY_HPP