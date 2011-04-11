#ifndef GAMEOVERMENU_HPP
#define GAMEOVERMENU_HPP

#include "scenes/BaseMenu.hpp"

class GameOverMenu: public BaseMenu
{
public:
	GameOverMenu();

	// inherited
	void OnFocus();

private:
	// inherited
	void EventCallback(int id);

	void UploadScore();

	int score_;
	gui::Label* lab_result_;
	CosmoButton* but_send_score_;
	gui::Label* lab_pseudo_;
	gui::TextBox* txt_pseudo_;
};

#endif // GAMEOVERMENU_HPP

