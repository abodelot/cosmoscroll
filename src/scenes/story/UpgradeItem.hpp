#ifndef UPGRADEITEM_HPP
#define UPGRADEITEM_HPP

#include "gui/Widget.hpp"
#include "items/ItemData.hpp"

class UpgradeItem: public gui::Widget
{
public:
	UpgradeItem(gui::Menu* parent, ItemData::Type type);

	// inherited callbacks
	void OnKeyPressed(sf::Keyboard::Key code);
	void OnMouseClicked(int x, int y);

	/**
	 * Refresh label content (name and level)
	 */
	void RefreshLabel();

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	ItemData::Type type_;
	xsf::Text label_;
	xsf::Sprite halo_;
	sf::RectangleShape label_bg_;
};

#endif // UPGRADEITEM_HPP
