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
	static const int MAX_LEVEL = 3;

	UpgradeItem(gui::Menu* parent, Type type);

	// inherited callbacks
	void OnKeyPressed(sf::Key::Code code);
	void OnMouseClicked(int x, int y);

	static const char* TypeToString(Type type);


private:
	void Render(sf::RenderTarget& target) const;


	Type type_;
	sf::String label_;
	sf::Sprite halo_;
	sf::Sprite label_bg_;
};

#endif // UPGRADEITEM_HPP
