#include "Drawable.hpp"

float Drawable::getScale() {
	return m_scale;
}

void Drawable::setScale(float scale) {
	m_scale = scale;
}

SDL_Point Drawable::getRelativePosition() {
	return m_relativePosition;
}

void Drawable::setRelativePosition(SDL_Point relativePosition) {
	m_relativePosition = relativePosition;
}

void Drawable::setRelativePosition(int positionX, int positionY) {
	SDL_Point position = {positionX, positionY};
	setRelativePosition(position);
}

SDL_Point Drawable::getAbsolutePosition() {
	SDL_Point parentPosition = {0, 0};
	SDL_Point myPosition = getRelativePosition();
	if (m_parent != NULL)
		parentPosition = m_parent->getAbsolutePosition();

	SDL_Point absolutePosition;
	absolutePosition.x = myPosition.x + parentPosition.x;
	absolutePosition.y = myPosition.y + parentPosition.y;

	return absolutePosition;
}

Drawable* Drawable::getParent() {
	return m_parent;
}

void Drawable::setParent(Drawable* parent) {
	m_parent = parent;
}