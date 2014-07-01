#ifndef LEADERBOARDMENU_HPP
#define LEADERBOARDMENU_HPP

#include "scenes/BaseMenu.hpp"

class LeaderboardMenu: public BaseMenu
{
public:
	LeaderboardMenu();

	void OnFocus() override;
	void Update(float frametime) override;
	void Show(sf::RenderTarget& target) const override;

private:
	void EventCallback(int id) override;

	sf::Text m_content;

	enum QueryStatus
	{
		NOT_STARTED,
		IN_PROGRESS,
		DONE
	};
	QueryStatus querying_;
};

#endif // LEADERBOARDMENU_HPP
