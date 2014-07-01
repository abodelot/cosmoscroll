#ifndef SENDSCOREMENU_HPP
#define SENDSCOREMENU_HPP

#include "scenes/BaseMenu.hpp"

class SendScoreMenu: public BaseMenu
{
public:
	SendScoreMenu();

	void OnFocus() override;

private:
	void EventCallback(int id) override;

	void uploadScore();

	int score_;
	gui::Label* lab_result_;
	CosmoButton* but_send_score_;
	CosmoButton* but_commit_;
	gui::Label* lab_pseudo_;
	gui::TextBox* txt_pseudo_;
};

#endif // SENDSCOREMENU_HPP

