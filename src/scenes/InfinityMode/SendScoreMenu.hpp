#ifndef SENDSCOREMENU_HPP
#define SENDSCOREMENU_HPP

#include "scenes/BaseMenu.hpp"

/**
 * Ask player name and upload score to the leaderboard
 */
class SendScoreMenu: public BaseMenu
{
public:
	SendScoreMenu();

	void onFocus() override;

private:
	void EventCallback(int id) override;

	void uploadScore();

	int m_score;
	gui::Label* lab_result_;
	CosmoButton* but_send_score_;
	CosmoButton* but_commit_;
	gui::Label* lab_pseudo_;
	gui::TextBox* txt_pseudo_;
};

#endif // SENDSCOREMENU_HPP

