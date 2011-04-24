#ifndef BESTSCORESMENU_HPP
#define BESTSCORESMENU_HPP

#include "scenes/BaseMenu.hpp"

class BestScoresMenu: public BaseMenu
{
public:
	BestScoresMenu();
	// inherited
	void OnFocus();

	void Update(float frametime);
	void Show(sf::RenderTarget& target) const;

private:
	// inherited
	void EventCallback(int id);

	gui::BitmapString* lab_content_;
	enum QueryStatus
	{
		NOT_STARTED,
		IN_PROGRESS,
		DONE
	};
	QueryStatus querying_;
};

#endif // BESTSCORESMENU_HPP
