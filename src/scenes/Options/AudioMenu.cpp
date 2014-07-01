#include "AudioMenu.hpp"
#include "core/Game.hpp"
#include "core/SoundSystem.hpp"
#include "utils/I18n.hpp"


AudioMenu::AudioMenu()
{
	SetTitle(_t("audio.title"));

	gui::FormLayout form(60, 120);
	form.SetSpacing(40, 20);

	cb_music_ = new gui::CheckBox(this);
	cb_music_->Check(SoundSystem::isMusicEnabled());
	cb_music_->SetCallbackID(2);
	form.AddRow(_t("audio.enable_music"), cb_music_);

	sl_music_vol_ = new gui::Slider(this, 200);
	sl_music_vol_->SetValue(SoundSystem::getMusicVolume());
	sl_music_vol_->SetCallbackID(3);
	form.AddRow(_t("audio.music_volume"), sl_music_vol_);

	cb_sound_ = new gui::CheckBox(this);
	cb_sound_->Check(SoundSystem::isSoundEnabled());
	cb_sound_->SetCallbackID(4);
	form.AddRow(_t("audio.enable_sound"), cb_sound_);

	sl_sound_vol_ = new gui::Slider(this, 200);
	sl_sound_vol_->SetValue(SoundSystem::getSoundVolume());
	sl_sound_vol_->SetCallbackID(5);
	form.AddRow(_t("audio.sound_volume"), sl_sound_vol_);

	gui::Button* b = new CosmoButton(this, _t("back"));
	b->setPosition(210, 335);
	b->SetCallbackID(0);
}


void AudioMenu::EventCallback(int id)
{
	switch (id)
	{
		case 0:
			Game::getInstance().setNextScene(Game::SC_OptionMenu);
			break;
		case 2:
			SoundSystem::enableMusic(cb_music_->Checked());
			break;
		case 3:
			SoundSystem::setMusicVolume(sl_music_vol_->GetValue());
			break;
		case 4:
			SoundSystem::enableSound(cb_sound_->Checked());
			break;
		case 5:
			SoundSystem::setSoundVolume(sl_sound_vol_->GetValue());
			break;
	}
}
