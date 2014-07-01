#include "PauseMenu.hpp"
#include "core/Input.hpp"
#include "core/Game.hpp"
#include "core/ControlPanel.hpp"
#include "core/ParticleSystem.hpp"
#include "core/SoundSystem.hpp"
#include "entities/EntityManager.hpp"
#include "utils/I18n.hpp"


PauseMenu::PauseMenu()
{
	SetTitle(_t("pause.title"), 120);
	SetBackground(sf::Sprite());

	gui::VBoxLayout layout(210, 200);
	layout.Add(new CosmoButton(this, _t("pause.resume")))->SetCallbackID(1);
	layout.Add(new CosmoButton(this, _t("back_main_menu")))->SetCallbackID(2);
	layout.Add(new CosmoButton(this, _t("pause.quit")))->SetCallbackID(3);
}


void PauseMenu::OnEvent(const sf::Event& event)
{
	Action::ID action = Input::feedEvent(event);
	if (action == Action::PAUSE)
	{
		 // Resume game
		SoundSystem::playMusic();
		Game::getInstance().setNextScene(Game::SC_PlayScreen);
	}
	else
	{
		BaseMenu::OnEvent(event);
	}
}


void PauseMenu::OnFocus()
{
	Game::getInstance().getWindow().setMouseCursorVisible(true);
	Game::getInstance().getWindow().setKeyRepeatEnabled(true);

	SoundSystem::pauseMusic();
}


void PauseMenu::EventCallback(int id)
{
	Game& game = Game::getInstance();
	switch (id)
	{
		case 1:
			game.setNextScene(Game::SC_PlayScreen);
			SoundSystem::playMusic();
			break;
		case 2:
			game.setNextScene(Game::SC_MainMenu);
			break;
		case 3:
			game.quit();
			break;
	}
}


void PauseMenu::Update(float frametime)
{
	ParticleSystem::getInstance().update(frametime);
}


void PauseMenu::Show(sf::RenderTarget& target) const
{
	target.draw(ControlPanel::getInstance());
	target.draw(EntityManager::getInstance());
	target.draw(GetTitle());
	gui::Menu::Show(target);
}

