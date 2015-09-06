#include "MainMenu.hpp"
#include "core/Game.hpp"
#include "core/Constants.hpp"
#include "core/SoundSystem.hpp"
#include "core/Resources.hpp"
#include "utils/I18n.hpp"


MainMenu::MainMenu()
{
	m_title.setTexture(Resources::getTexture("gui/cosmoscroll-logo.png"));
	m_title.setPosition((APP_WIDTH - m_title.getLocalBounds().width) / 2, 12);

	gui::VBoxLayout layout(210, 120);
	layout.SetSpacing(0, 10);

	layout.Add(new CosmoButton(this, _t("main.levels")))->SetCallbackID(1);
	layout.Add(new CosmoButton(this, _t("main.infinity")))->SetCallbackID(2);
	layout.Add(new CosmoButton(this, _t("main.options")))->SetCallbackID(3);
	layout.Add(new CosmoButton(this, _t("main.about")))->SetCallbackID(4);
	layout.Add(new CosmoButton(this, _t("main.quit")))->SetCallbackID(5);
}


void MainMenu::draw(sf::RenderTarget& target) const
{
	BaseMenu::draw(target);
	target.draw(m_title);
}


void MainMenu::EventCallback(int id)
{
	Game& game = Game::getInstance();
	switch (id)
	{
		case 1:
			game.setCurrentScreen(Game::SC_LevelMenu);
			break;
		case 2:
			game.setCurrentScreen(Game::SC_InfinityModeMenu);
			break;
		case 3:
			game.setCurrentScreen(Game::SC_OptionMenu);
			break;
		case 4:
			game.setCurrentScreen(Game::SC_AboutMenu);
			break;
		case 5:
			game.quit();
			break;
	}
}
