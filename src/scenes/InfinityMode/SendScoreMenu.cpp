#include <SFML/Network.hpp>

#include "SendScoreMenu.hpp"
#include "core/Game.hpp"
#include "core/Constants.hpp"
#include "core/UserSettings.hpp"
#include "core/Resources.hpp"
#include "entities/EntityManager.hpp"
#include "entities/Player.hpp"
#include "utils/I18n.hpp"
#include "utils/md5/md5.hpp"


SendScoreMenu::SendScoreMenu():
	m_score(0)
{
	setTitle(_t("sendscore.title"));

	lab_result_ = new gui::Label(this, "", 120, 120);
	lab_result_->setCharacterSize(30);

	lab_pseudo_ = new gui::Label(this, _t("sendscore.pseudo"), 120, 200);

	txt_pseudo_ = new gui::TextBox(this, 240, 200, 20, 40);
	txt_pseudo_->SetCallbackID(3);

	but_commit_ = new CosmoButton(this, _t("submit"));
	but_commit_->setPosition(210, 240);
	but_commit_->SetCallbackID(3);

	but_send_score_ = new CosmoButton(this, _t("sendscore.send_score"));
	but_send_score_->setPosition(210, 240);
	but_send_score_->SetCallbackID(0);

	gui::Button* b = new CosmoButton(this, _t("sendscore.play_again"));
	b->setPosition(210, 290);
	b->SetCallbackID(1);
	b = new CosmoButton(this, _t("back_main_menu"));
	b->setPosition(210, 340);
	b->SetCallbackID(2);
}


void SendScoreMenu::onFocus()
{
	// Get player score and check if new highscore
	m_score = EntityManager::getInstance().getPlayer()->getScore();
	if (m_score > UserSettings::getHighscore())
	{
		UserSettings::setHighscore(m_score);
		lab_result_->setString(I18n::templatize("sendscore.new_record", "{score}", m_score));
	}
	else
	{
		lab_result_->setString(I18n::templatize("sendscore.no_record", "{score}", m_score));
	}
	lab_result_->setCharacterSize(30);

	but_send_score_->SetVisible(true);
	FocusWidget(but_send_score_);
	lab_pseudo_->SetVisible(false);
	txt_pseudo_->SetVisible(false);
	but_commit_->SetVisible(false);
}


void SendScoreMenu::EventCallback(int id)
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
			EntityManager::getInstance().setMode(EntityManager::INFINITY_MODE);
			ControlPanel::getInstance().setHighscore(UserSettings::getHighscore());
			Game::getInstance().setCurrentScreen(Game::SC_PlayScreen);
			break;
		case 2:
			Game::getInstance().setCurrentScreen(Game::SC_MainMenu);
			break;
		case 3:
			if (EntityManager::getInstance().getPlayer()->isCheater())
			{
				lab_result_->setCharacterSize(20);
				lab_result_->setString(_t("sendscore.error_cheat"));
			}
			else if (!Game::getInstance().resourcesChecked())
			{
				lab_result_->setCharacterSize(20);
				lab_result_->setString(_t("sendscore.error_altered_res"));
			}
			else
			{
				uploadScore();
			}
			break;
	}
}


void SendScoreMenu::uploadScore()
{
	std::string str_name = str_trim(txt_pseudo_->GetText());
	if (str_name.empty())
		return;

	std::string str_score = std::to_string(m_score);

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
	                 + "&version=" + APP_VERSION;
	request.setBody(body);

	// Send it and get the response returned by the server
	sf::Http::Response response = server.sendRequest(request);
	switch (response.getStatus())
	{
		case sf::Http::Response::Ok:
			Game::getInstance().setCurrentScreen(Game::SC_LeaderboardMenu);
			break;
		case sf::Http::Response::ConnectionFailed:
			lab_result_->setString("Couldn't connect to CosmoScroll server");
			break;
		default:
			lab_result_->setString(response.getBody());
			break;
	}
}
