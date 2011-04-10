#include <SFML/Network.hpp>

#include "GameOverMenu.hpp"
#include "../core/Game.hpp"
#include "../entities/EntityManager.hpp"
#include "../utils/MediaManager.hpp"
#include "../utils/StringUtils.hpp"
#include "../utils/I18n.hpp"
#include "../md5/md5.hpp"

#define COSMO_KEY "not published"

GameOverMenu::GameOverMenu()
{
	LoadBitmapFont("data/images/gui/mono12-black.png", 10, 10);

	SetTitle(I18n::t("menu.gameover.title"));

	score_ = 0;
	lab_result_ = new gui::Label(this, "", 120, 150);
	lab_result_->SetSize(30);

	but_send_score_ = new CosmoButton(this, I18n::t("menu.gameover.send_score"), 210, 240);
	but_send_score_->SetCallbackID(0);
	(new CosmoButton(this, I18n::t("menu.gameover.play_again"), 210, 290))->SetCallbackID(1);
	(new CosmoButton(this, I18n::t("menu.back_main_menu"), 210, 340))->SetCallbackID(2);

	lab_pseudo_ = new gui::Label(this, I18n::t("menu.gameover.pseudo"), 100, 250);
	lab_pseudo_->SetState(gui::State::HIDDEN);
	txt_pseudo_ = new gui::TextBox(this, 210, 250, 30);
	txt_pseudo_->SetCallbackID(3);
}


void GameOverMenu::OnFocus()
{
	BaseMenu::OnFocus();
	EntityManager& entities = EntityManager::GetInstance();

	score_ = (int) entities.GetTimer();
	int min = score_ / 60;
	int sec = score_ % 60;
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
	lab_result_->SetText(text);

	but_send_score_->SetState(gui::State::DEFAULT);
	FocusWidget(but_send_score_);
	txt_pseudo_->SetState(gui::State::HIDDEN);
}


void GameOverMenu::EventCallback(int id)
{
	switch (id)
	{
		case 0:
			txt_pseudo_->SetState(gui::State::DEFAULT);
			lab_pseudo_->SetState(gui::State::DEFAULT);
			FocusWidget(txt_pseudo_);
			but_send_score_->SetState(gui::State::HIDDEN);
			break;
		case 1:
			EntityManager::GetInstance().InitMode(EntityManager::MODE_ARCADE);
			Game::GetInstance().SetNextScene(Game::SC_InGameScene);
			break;
		case 2:
			Game::GetInstance().SetNextScene(Game::SC_MainMenu);
			break;
		case 3:
			if (Game::GetInstance().IsPure())
			{
				UploadScore();
			}
			else
			{
				lab_result_->SetText("Petit malin...");
			}
			break;
	}
}


void GameOverMenu::UploadScore()
{
	std::string str_name = txt_pseudo_->GetText();
	std::string str_score = str_sprintf("%d", score_);

	// COSMO_KEY is a salt, producing a different salted key for each couple (name, score)
	MD5 key(str_name + str_score + COSMO_KEY);

	// Connect to cosmoscroll scores server
	sf::Http server;
	server.SetHost(COSMO_HOSTNAME);

	// Submit the score
	sf::Http::Request request;
	request.SetMethod(sf::Http::Request::Post);
	request.SetURI(COSMO_URI);
	std::string body = "name=" + str_name + "&score=" + str_score + "&key=" + key.GetHash();
	request.SetBody(body);

	// Send it and get the response returned by the server
	sf::Http::Response response = server.SendRequest(request);
	switch (response.GetStatus())
	{
		case sf::Http::Response::Ok:
			Game::GetInstance().SetNextScene(Game::SC_BestScoresMenu);
			break;
		case sf::Http::Response::ConnectionFailed:
			lab_result_->SetText("Couldn't connect to CosmoScroll server");
			break;
		default:
			lab_result_->SetText(response.GetBody());
			break;
	}
}
