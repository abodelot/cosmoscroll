#ifndef UPGRADEITEM_HPP
#define UPGRADEITEM_HPP

#include "gui/Widget.hpp"
#include "items/ItemData.hpp"

class UpgradeItem: public gui::Widget
{
public:
	UpgradeItem(gui::Menu* parent, ItemData::Type type);

	// inherited callbacks
	void OnKeyPressed(sf::Key::Code code);
	void OnMouseClicked(int x, int y);
private:
	void Render(sf::RenderTarget& target) const;


	ItemData::Type type_;
	sf::String label_;
	sf::Sprite halo_;
	sf::Sprite label_bg_;
};

#endif // UPGRADEITEM_HPP
