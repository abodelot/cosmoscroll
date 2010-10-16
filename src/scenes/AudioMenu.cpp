#include "AudioMenu.hpp"
#include "../core/Game.hpp"
#include "../core/SoundSystem.hpp"
#include "../utils/I18n.hpp"


AudioMenu::AudioMenu()
{
	SetTitle(I18n::t("menu.audio.title"));
	SoundSystem& sound = SoundSystem::GetInstance();

	new gui::Label(this, L"Musique :", 100, 120);

	opt_music_ = new gui::OptionList(this, 350, 120);
	opt_music_->AddOption("Space", "space");
	opt_music_->AddOption("Aurora", "aurora");
	opt_music_->AddOption("Escape for assault", "escape_for_assault");
	opt_music_->SetCallbackID(1);
	const std::string music_name = sound.GetMusicName();
	if (music_name == "space")
		opt_music_->Select(0);
	else if (music_name == "aurora")
		opt_music_->Select(1);
	else if (music_name == "escape_for_assault")
		opt_music_->Select(2);

	new gui::Label(this, L"Activer la musique :", 100, 170);
	cb_music_ = new gui::CheckBox(this, 350, 170);
	cb_music_->Check(sound.IsMusicEnabled());
	cb_music_->SetCallbackID(2);

	new gui::Label(this, L"Volume de la musique :", 100, 200);
	sl_music_vol_ = new gui::Slider(this, 350, 200, 200);
	sl_music_vol_->SetValue(sound.GetMusicVolume());
	sl_music_vol_->SetCallbackID(3);

	new gui::Label(this, L"Activer les sons :", 100, 260);
	cb_sound_ = new gui::CheckBox(this, 350, 260);
	cb_sound_->Check(sound.IsSoundEnabled());
	cb_sound_->SetCallbackID(4);

	new gui::Label(this, L"Volume des sons :", 100, 290);
	sl_sound_vol_ = new gui::Slider(this, 350, 290, 200);
	sl_sound_vol_->SetValue(sound.GetSoundVolume());
	sl_sound_vol_->SetCallbackID(5);

	(new CosmoButton(this, I18n::t("menu.back"), 210, 335))->SetCallbackID(0);
}


void AudioMenu::EventCallback(int id)
{
	SoundSystem& sound = SoundSystem::GetInstance();
	switch (id)
	{
		case 0:
			Game::GetInstance().SetNextScene(Game::SC_OptionMenu);
			break;
		case 1:
			sound.PlayMusic(opt_music_->GetSelectedOption());
			break;
		case 2:
			sound.EnableMusic(cb_music_->Checked());
			break;
		case 3:
			sound.SetMusicVolume(sl_music_vol_->GetValue());
			break;
		case 4:
			sound.EnableSound(cb_sound_->Checked());
			break;
		case 5:
			sound.SetSoundVolume(sl_sound_vol_->GetValue());
			break;
	}
}
