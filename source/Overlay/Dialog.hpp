#ifndef DIALOG_HPP
#define DIALOG_HPP

#include <TweenEngine/TweenManager.h>

#include "Overlay.hpp"

class Dialog: public Overlay {
public:
	Dialog(std::string message);

	void update(float delta) override;
	void processEvent(SDL_Event event) override;
	void render(RenderKit* renderKit) override;
};

#endif // DIALOG_HPP