#ifndef BROWSERITEM_HPP
#define BROWSERITEM_HPP

#include <string>

#include "Text.hpp"
#include "../Entry/Entry.hpp"
#include "../TweenObjects.hpp"

class BrowserItem: public TweenDrawable {
public:
	BrowserItem(Drawable* parent, Entry* entry, SDL_Point pos);

	void draw(SDL_Renderer* renderer, TTF_Font* font) override;

	bool isSelected();
	void setSelected(bool selected);
private:
	Entry* m_entry;
	Text m_entryName;
	SDL_Texture* m_icon;

	bool m_selected;
};

#endif // BROWSERITEM_HPP