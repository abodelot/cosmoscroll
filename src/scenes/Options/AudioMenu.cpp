#include "AudioMenu.hpp"
#include "core/Game.hpp"
#include "core/SoundSystem.hpp"
#include "core/Services.hpp"


AudioMenu::AudioMenu()
{
    setTitle(_t("audio.title"));

    gui::FormLayout form(90, 120);
    form.setSpacing(20, 20);

    // Checkbox toggle music
    cb_music_ = new gui::CheckBox(this);
    cb_music_->Check(Services::getSoundSystem().isMusicEnabled());
    cb_music_->setCallback([this]() {
        Services::getSoundSystem().enableMusic(cb_music_->Checked());
    });
    form.addRow(_t("audio.enable_music"), cb_music_);

    // Slider music volume
    sl_music_vol_ = new gui::Slider(this, 200);
    sl_music_vol_->setValue(Services::getSoundSystem().getMusicVolume());
    sl_music_vol_->setCallback([this]() {
        Services::getSoundSystem().setMusicVolume(sl_music_vol_->getValue());
    });
    form.addRow(_t("audio.music_volume"), sl_music_vol_);

    // Checkbox toggle sfx
    cb_sound_ = new gui::CheckBox(this);
    cb_sound_->Check(Services::getSoundSystem().isSoundEnabled());
    cb_sound_->setCallback([this]() {
        Services::getSoundSystem().enableSound(cb_sound_->Checked());
    });
    form.addRow(_t("audio.enable_sound"), cb_sound_);

    // Slider sfx volume
    sl_sound_vol_ = new gui::Slider(this, 200);
    sl_sound_vol_->setValue(Services::getSoundSystem().getSoundVolume());
    sl_sound_vol_->setCallback([this]() {
        Services::getSoundSystem().setSoundVolume(sl_sound_vol_->getValue());
    });
    form.addRow(_t("audio.sound_volume"), sl_sound_vol_);

    // Back button
    gui::Button* b = new CosmoButton(this, _t("back"));
    b->setPosition(210, 335);
    b->setCallback([this]() {
        Game::getInstance().setCurrentScreen(Game::SC_OptionMenu);
    });
}
