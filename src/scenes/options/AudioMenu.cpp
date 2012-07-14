#include "AudioMenu.hpp"
#include "core/Game.hpp"
#include "core/SoundSystem.hpp"
#include "utils/I18n.hpp"


AudioMenu::AudioMenu()
{
	SetTitle(_t("menu.audio.title"));
	SoundSystem& sound = SoundSystem::GetInstance();

	gui::FormLayout form(60, 120);
	form.SetSpacing(40, 20);

	cb_music_ = new gui::CheckBox(this);
	cb_music_->Check(sound.IsMusicEnabled());
	cb_music_->SetCallbackID(2);
	form.AddRow(_t("menu.audio.enable_music"), cb_music_);

	sl_music_vol_ = new gui::Slider(this, 200);
	sl_music_vol_->SetValue(sound.GetMusicVolume());
	sl_music_vol_->SetCallbackID(3);
	form.AddRow(_t("menu.audio.music_volume"), sl_music_vol_);

	cb_sound_ = new gui::CheckBox(this);
	cb_sound_->Check(sound.IsSoundEnabled());
	cb_sound_->SetCallbackID(4);
	form.AddRow(_t("menu.audio.enable_sound"), cb_sound_);

	sl_sound_vol_ = new gui::Slider(this, 200);
	sl_sound_vol_->SetValue(sound.GetSoundVolume());
	sl_sound_vol_->SetCallbackID(5);
	form.AddRow(_t("menu.audio.sound_volume"), sl_sound_vol_);

	gui::Button* b = new CosmoButton(this, _t("menu.back"));
	b->SetPosition(210, 335);
	b->SetCallbackID(0);
}


void AudioMenu::EventCallback(int id)
{
	SoundSystem& sound = SoundSystem::GetInstance();
	switch (id)
	{
		case 0:
			Game::GetInstance().SetNextScene(Game::SC_OptionMenu);
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
