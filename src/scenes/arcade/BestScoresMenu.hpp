#ifndef BESTSCORESMENU_HPP
#define BESTSCORESMENU_HPP

#include "scenes/BaseMenu.hpp"

class BestScoresMenu: public BaseMenu
{
public:
	BestScoresMenu();

	void OnFocus() override;
	void Update(float frametime);
	void Show(sf::RenderTarget& target) const;

private:
	void EventCallback(int id) override;

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
