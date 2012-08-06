#include <SFML/Network.hpp>

#include "GameOverMenu.hpp"
#include "core/Game.hpp"
#include "core/Constants.hpp"
#include "entities/EntityManager.hpp"
#include "entities/PlayerShip.hpp"
#include "utils/StringUtils.hpp"
#include "utils/I18n.hpp"
#include "md5/md5.hpp"


GameOverMenu::GameOverMenu()
{
	LoadBitmapFont(Resources::getTexture("gui/mono12-black.png"), 10, 10);
	SetTitle(_t("menu.gameover.title"));

	score_ = 0;
	lab_result_ = new gui::Label(this, "", 120, 120);
	lab_result_->setCharacterSize(30);

	lab_pseudo_ = new gui::Label(this, _t("menu.gameover.pseudo"), 100, 200);
	txt_pseudo_ = new gui::TextBox(this, 210, 200, 30);
	txt_pseudo_->SetCallbackID(3);

	but_commit_ = new CosmoButton(this, _t("menu.submit"));
	but_commit_->setPosition(210, 240);
	but_commit_->SetCallbackID(3);

	but_send_score_ = new CosmoButton(this, _t("menu.gameover.send_score"));
	but_send_score_->setPosition(210, 240);
	but_send_score_->SetCallbackID(0);

	gui::Button* b = new CosmoButton(this, _t("menu.gameover.play_again"));
	b->setPosition(210, 290);
	b->SetCallbackID(1);
	b = new CosmoButton(this, _t("menu.back_main_menu"));
	b->setPosition(210, 340);
	b->SetCallbackID(2);
}


void GameOverMenu::OnFocus()
{
	BaseMenu::OnFocus();
	EntityManager& entities = EntityManager::GetInstance();

	score_ = entities.GetPlayerShip()->GetPoints();

	std::wstring text;
	if (score_ > entities.GetArcadeRecord())
	{
		entities.UpdateArcadeRecord();
		text = wstr_replace(_t("menu.gameover.new_record"), L"{score}", to_wstring(score_));
	}
	else
	{
		text = wstr_replace(_t("menu.gameover.no_record"), L"{score}", to_wstring(score_));
	}
	lab_result_->setCharacterSize(30);
	lab_result_->setString(text);

	but_send_score_->SetVisible(true);
	FocusWidget(but_send_score_);
	lab_pseudo_->SetVisible(false);
	txt_pseudo_->SetVisible(false);
	but_commit_->SetVisible(false);
}


void GameOverMenu::EventCallback(int id)
{
	switch (id)
	{
		case 0:
			but_commit_->SetVisible(true);
			txt_pseudo_->SetVisible(true);
			lab_pseudo_->SetVisible(true);
			FocusWidget(txt_pseudo_);
			but_send_score_->SetVisible(false);
			break;
		case 1:
			EntityManager::GetInstance().InitMode(EntityManager::MODE_ARCADE);
			Game::GetInstance().SetNextScene(Game::SC_InGameScene);
			break;
		case 2:
			Game::GetInstance().SetNextScene(Game::SC_MainMenu);
			break;
		case 3:
			if (EntityManager::GetInstance().GetPlayerShip()->HasCheated())
			{
				lab_result_->setCharacterSize(20);
				lab_result_->setString(_t("menu.gameover.error_cheat"));
			}
			else if (!Game::GetInstance().IsPure())
			{
				lab_result_->setCharacterSize(20);
				lab_result_->setString(_t("menu.gameover.error_altered_res"));
			}
			else
			{
				UploadScore();
			}
			break;
	}
}


void GameOverMenu::UploadScore()
{
	std::string str_name = str_trim(txt_pseudo_->GetText());
	if (str_name.empty())
		return;

	std::string str_score = to_string(score_);

	// COSMO_SERVER_KEY is a salt, producing a different salted key for each couple (name, score)
	MD5 key(str_name + str_score + COSMO_SERVER_KEY);

	// Connect to cosmoscroll scores server
	sf::Http server;
	server.setHost(COSMO_SERVER_HOSTNAME);

	// Submit the score
	sf::Http::Request request;
	request.setMethod(sf::Http::Request::Post);
	request.setUri(COSMO_SERVER_URI);
	std::string body = "name=" + str_name
	                 + "&score=" + str_score
	                 + "&key=" + key.GetHash()
	                 + "&version=" + GAME_VERSION;
	request.setBody(body);

	// Send it and get the response returned by the server
	sf::Http::Response response = server.sendRequest(request);
	switch (response.getStatus())
	{
		case sf::Http::Response::Ok:
			Game::GetInstance().SetNextScene(Game::SC_BestScoresMenu);
			break;
		case sf::Http::Response::ConnectionFailed:
			lab_result_->setString("Couldn't connect to CosmoScroll server");
			break;
		default:
			lab_result_->setString(response.getBody());
			break;
	}
}
