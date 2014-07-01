#include "MainMenu.hpp"
#include "core/Game.hpp"
#include "core/Constants.hpp"
#include "core/SoundSystem.hpp"
#include "core/Resources.hpp"
#include "utils/I18n.hpp"
#include "utils/SFML_Helper.hpp"


MainMenu::MainMenu()
{
	title_.setTexture(Resources::getTexture("gui/cosmoscroll-logo.png"));
	title_.setPosition((APP_WIDTH - sfh::width(title_)) / 2, 12);

	gui::VBoxLayout layout(210, 120);
	layout.SetSpacing(0, 10);

	layout.Add(new CosmoButton(this, _t("main.levels")))->SetCallbackID(1);
	layout.Add(new CosmoButton(this, _t("main.infinity")))->SetCallbackID(2);
	layout.Add(new CosmoButton(this, _t("main.options")))->SetCallbackID(3);
	layout.Add(new CosmoButton(this, _t("main.about")))->SetCallbackID(4);
	layout.Add(new CosmoButton(this, _t("main.quit")))->SetCallbackID(5);
}


void MainMenu::Show(sf::RenderTarget& target) const
{
	BaseMenu::Show(target);
	target.draw(title_);
}


void MainMenu::EventCallback(int id)
{
	Game& game = Game::getInstance();
	switch (id)
	{
		case 1:
			game.setNextScene(Game::SC_LevelMenu);
			break;
		case 2:
			game.setNextScene(Game::SC_InfinityModeMenu);
			break;
		case 3:
			game.setNextScene(Game::SC_OptionMenu);
			break;
		case 4:
			game.setNextScene(Game::SC_AboutMenu);
			break;
		case 5:
			game.quit();
			break;
	}
}
