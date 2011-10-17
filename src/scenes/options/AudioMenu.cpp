#include "AudioMenu.hpp"
#include "core/Game.hpp"
#include "core/SoundSystem.hpp"
#include "utils/I18n.hpp"


AudioMenu::AudioMenu()
{
	SetTitle(_t("menu.audio.title"));
	SoundSystem& sound = SoundSystem::GetInstance();

	new gui::Label(this, _t("menu.audio.music"), 100, 120);

	opt_music_ = new gui::OptionList(this, 345, 120);
	opt_music_->AddOption("Space", "space.mod");
	opt_music_->AddOption("Aurora", "aurora.mod");
	opt_music_->AddOption("Escape for assault", "escape_for_assault.mod");
	opt_music_->SetCallbackID(1);
	const std::string music_name = sound.GetMusicName();
	if (music_name == "space")
		opt_music_->Select(0);
	else if (music_name == "aurora")
		opt_music_->Select(1);
	else if (music_name == "escape_for_assault")
		opt_music_->Select(2);

	new gui::Label(this, _t("menu.audio.enable_music"), 100, 170);
	cb_music_ = new gui::CheckBox(this, 345, 170);
	cb_music_->Check(sound.IsMusicEnabled());
	cb_music_->SetCallbackID(2);

	new gui::Label(this, _t("menu.audio.music_volume"), 100, 200);
	sl_music_vol_ = new gui::Slider(this, 345, 200, 200);
	sl_music_vol_->SetValue(sound.GetMusicVolume());
	sl_music_vol_->SetCallbackID(3);

	new gui::Label(this, _t("menu.audio.enable_sound"), 100, 260);
	cb_sound_ = new gui::CheckBox(this, 345, 260);
	cb_sound_->Check(sound.IsSoundEnabled());
	cb_sound_->SetCallbackID(4);

	new gui::Label(this, _t("menu.audio.sound_volume"), 100, 290);
	sl_sound_vol_ = new gui::Slider(this, 345, 290, 200);
	sl_sound_vol_->SetValue(sound.GetSoundVolume());
	sl_sound_vol_->SetCallbackID(5);

	(new CosmoButton(this, _t("menu.back"), 210, 335))->SetCallbackID(0);
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
