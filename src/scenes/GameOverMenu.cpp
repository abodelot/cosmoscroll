#include "GameOverMenu.hpp"
#include "../core/Game.hpp"
#include "../entities/EntityManager.hpp"
#include "../utils/MediaManager.hpp"
#include "../utils/StringUtils.hpp"
#include "../utils/I18n.hpp"


GameOverMenu::GameOverMenu()
{
	LoadBitmapFont("data/images/gui/mono12-black.png", 10, 10);

	SetTitle(I18n::t("menu.gameover.title"));

	result_ = new gui::Label(this, "", 120, 150);
	result_->SetSize(30);

	but_send_score_ = new CosmoButton(this, "Envoyer score", 210, 250);
	but_send_score_->SetCallbackID(0);
	(new CosmoButton(this, I18n::t("menu.gameover.play_again"), 210, 290))->SetCallbackID(1);
	(new CosmoButton(this, I18n::t("menu.back_main_menu"), 210, 340))->SetCallbackID(2);

	txt_ = new gui::TextBox(this, 210, 250, 30);
}


void GameOverMenu::Poke()
{
	EntityManager& entities = EntityManager::GetInstance();

	int min = (int) entities.GetTimer() / 60;
	int sec = (int) entities.GetTimer() % 60;
	std::wstring text;
	if (entities.GetTimer() > entities.GetArcadeRecord())
	{
		entities.UpdateArcadeRecord();
		text = str_sprintf(I18n::t("menu.gameover.record").c_str(), min, sec);
	}
	else
	{
		text = str_sprintf(I18n::t("menu.gameover.no_record").c_str(), min, sec);
	}
	result_->SetText(text);

	but_send_score_->SetState(gui::State::DEFAULT);
	FocusWidget(but_send_score_);
	txt_->SetState(gui::State::HIDDEN);
}


void GameOverMenu::EventCallback(int id)
{
	switch (id)
	{
		case 0:
			txt_->SetState(gui::State::DEFAULT);
			FocusWidget(txt_);
			but_send_score_->SetState(gui::State::HIDDEN);
			break;
		case 1:
			EntityManager::GetInstance().InitMode(EntityManager::MODE_ARCADE);
			Game::GetInstance().SetNextScene(Game::SC_InGameScene);
			break;
		case 2:
			Game::GetInstance().SetNextScene(Game::SC_MainMenu);
			break;
	}
}
