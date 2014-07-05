#include <SFML/Network.hpp>

#include "LeaderboardMenu.hpp"
#include "core/Game.hpp"
#include "core/Resources.hpp"
#include "core/Constants.hpp"
#include "utils/I18n.hpp"


LeaderboardMenu::LeaderboardMenu():
	m_querying(NOT_STARTED)
{
	setTitle(_t("leaderboard.title"));
	gui::Button* b = new CosmoButton(this, _t("back"));
	b->setPosition(210, 420);
	b->SetCallbackID(1);

	m_content.setCharacterSize(16);
	m_content.setFont(Resources::getFont("VeraMono.ttf"));
	m_content.setPosition(120, 100);
}


void LeaderboardMenu::EventCallback(int id)
{
	switch (id)
	{
		case 1:
			Game::getInstance().setCurrentScreen(Game::SC_InfinityModeMenu);
			break;
	}
}


void LeaderboardMenu::update(float frametime)
{
	if (m_querying == DONE)
	{
		BaseMenu::update(frametime);
		return;
	}
	if (m_querying == NOT_STARTED)
	{
		m_querying = IN_PROGRESS;
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
	uri += APP_VERSION;
	request.setUri(uri);

	// Send it and get the response returned by the server
	sf::Http::Response response = server.sendRequest(request);
	switch (response.getStatus())
	{
		case sf::Http::Response::Ok:
			m_content.setString(response.getBody());
			break;
		case sf::Http::Response::ConnectionFailed:
			m_content.setString("Error: couldn't connect to server");
			break;
		default:
			m_content.setString("Error: server did not properly respond (" + std::to_string(response.getStatus()) + ")");
			break;
	}
	m_querying = DONE;
}


void LeaderboardMenu::draw(sf::RenderTarget& target) const
{
	BaseMenu::draw(target);
	target.draw(m_content);
}


void LeaderboardMenu::onFocus()
{
	m_content.setString(_t("leaderboard.wait"));
	m_querying = NOT_STARTED;
}
