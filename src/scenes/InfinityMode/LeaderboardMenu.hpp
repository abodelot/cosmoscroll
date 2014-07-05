#ifndef LEADERBOARDMENU_HPP
#define LEADERBOARDMENU_HPP

#include "scenes/BaseMenu.hpp"

/**
 * Display online highscores
 */
class LeaderboardMenu: public BaseMenu
{
public:
	LeaderboardMenu();

	void onFocus() override;

	void update(float frametime) override;

	void draw(sf::RenderTarget& target) const override;

private:
	void EventCallback(int id) override;

	sf::Text m_content;

	enum QueryStatus
	{
		NOT_STARTED,
		IN_PROGRESS,
		DONE
	};
	QueryStatus m_querying;
};

#endif // LEADERBOARDMENU_HPP
