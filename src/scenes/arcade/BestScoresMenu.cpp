#include <SFML/Network.hpp>

#include "BestScoresMenu.hpp"
#include "core/Game.hpp"
#include "core/Constants.hpp"
#include "utils/I18n.hpp"
#include "utils/StringUtils.hpp"


BestScoresMenu::BestScoresMenu()
{
	SetTitle(_t("menu.best_scores.title"));
	gui::Button* b = new CosmoButton(this, _t("menu.back"));
	b->setPosition(210, 420);
	b->SetCallbackID(1);

	LoadBitmapFont(Resources::getTexture("gui/mono12-white.png"), 10, 10);
	lab_content_ = new gui::BitmapString(*GetBitmapFont());
	lab_content_->setPosition(120, 100);

	querying_ = NOT_STARTED;
}


void BestScoresMenu::EventCallback(int id)
{
	switch (id)
	{
		case 1:
			Game::GetInstance().SetNextScene(Game::SC_ArcadeMenu);
			break;
	}
}


void BestScoresMenu::Update(float frametime)
{
	if (querying_ == DONE)
	{
		BaseMenu::Update(frametime);
		return;
	}
	if (querying_ == NOT_STARTED)
	{
		querying_ = IN_PROGRESS;
		return;
	}
	// Connect to cosmoscroll scores server
	sf::Http server;
	server.setHost(COSMO_SERVER_HOSTNAME);

	// Prepare a request to retrieve the scores
	sf::Http::Request request;
	request.setMethod(sf::Http::Request::Get);
	std::string uri = COSMO_SERVER_URI;
	uri += "?version=";
	uri += GAME_VERSION;
	request.setUri(uri);

	// Send it and get the response returned by the server
	sf::Http::Response response = server.sendRequest(request);
	switch (response.getStatus())
	{
		case sf::Http::Response::Ok:
			lab_content_->setString(response.getBody());
			break;
		case sf::Http::Response::ConnectionFailed:
			lab_content_->setString("Error: couldn't connect to server");
			break;
		default:
			lab_content_->setString("Error: server did not properly respond (" + std::to_string(response.getStatus()) + ")");
			break;
	}
	querying_ = DONE;
}


void BestScoresMenu::Show(sf::RenderTarget& target) const
{
	BaseMenu::Show(target);
	target.draw(*lab_content_);
}


void BestScoresMenu::OnFocus()
{
	lab_content_->setString("Please wait...");
	querying_ = NOT_STARTED;
}
