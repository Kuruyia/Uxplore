#ifndef BROWSERLIST_HPP
#define BROWSERLIST_HPP

#define VISIBLE_ROWS 6
#define ITEM_PER_ROW 3
#define ITEM_WIDTH 396
#define ITEM_HEIGHT 89
#define ITEM_MARGIN_X 23
#define ITEM_MARGIN_Y 18
#define ITEM_OFFSET 23

#include <map>
#include <memory>
#include <vector>
#include <SDL2/SDL_image.h>
#include <TweenEngine/TweenManager.h>

#include "Entry/Entry.hpp"
#include "TweenObjects.hpp"
#include "GUI/BrowserItem.hpp"

class BrowserList: public TweenDrawable {
public:
	BrowserList();

	void draw(SDL_Renderer* renderer, TTF_Font* font) override;
	void update(float delta);

	void addItem(const Entry &entry);
	std::shared_ptr<BrowserItem> getItem(unsigned int index);
	void reset();

	std::shared_ptr<BrowserItem> getSelectedItem();
	unsigned int getSelectedItemIndex();
	void setSelectedItemIndex(unsigned int index);
	void moveSelectedItemIndex(int indexDelta);

private:
	SDL_Point getPositionFromIndex(unsigned int index);

	std::vector<std::shared_ptr<BrowserItem>> m_items;
	Text m_emptyListText;
	unsigned int m_selectedItemIndex;
	int m_firstRow;

	std::map<Entry::EntryType, SDL_Texture*> m_icons;
	TweenEngine::TweenManager m_tweenManager;
};

#endif // BROWSERLIST_HPP