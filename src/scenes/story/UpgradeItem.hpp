#ifndef UPGRADEITEM_HPP
#define UPGRADEITEM_HPP

#include "gui/Widget.hpp"

class UpgradeItem: public gui::Widget
{
public:
	enum Type
	{
		UP_ARMOR,
		UP_ENGINE,
		UP_SHIELD,
		UP_HEATSINK,
		UP_LASER1,
		UP_LASER2,
		_UP_COUNT
	};
	UpgradeItem(gui::Menu* parent, Type type);

private:
	void Render(sf::RenderTarget& target) const;

	Type type_;
	sf::String label_;
	sf::Sprite halo_;
};

#endif // UPGRADEITEM_HPP
