#ifndef BESTSCORESMENU_HPP
#define BESTSCORESMENU_HPP

#include "scenes/BaseMenu.hpp"

class BestScoresMenu: public BaseMenu
{
public:
	BestScoresMenu();
	// inherited
	void OnFocus();

	void Show(sf::RenderTarget& target) const;

private:
	// inherited
	void EventCallback(int id);

	gui::BitmapString* lab_content_;
};

#endif // BESTSCORESMENU_HPP
