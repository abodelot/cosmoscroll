#include "AudioMenu.hpp"
#include "../core/Game.hpp"
#include "../core/SoundSystem.hpp"


AudioMenu::AudioMenu()
{
	SetTitle(L"Musique");
	SetOffsetY(100);

	AddOption("Space Song", 1);
	AddOption("Aurora", 2);
	AddOption("Escape For Assault", 3);
	AddOption("Pas de musique", 4);
	AddOption("Retour", 0);
}


void AudioMenu::Callback(int id)
{
	switch (id)
	{
		case 0:
			Game::GetInstance().SetNextScene(Game::SC_OptionMenu);
			break;
		case 1:
			SoundSystem::GetInstance().PlayMusic("space", true);
			break;
		case 2:
			SoundSystem::GetInstance().PlayMusic("aurora", true);
			break;
		case 3:
			SoundSystem::GetInstance().PlayMusic("escape_for_assault", true);
			break;
		case 4:
			SoundSystem::GetInstance().EnableMusic(false);
			break;
	}
}
