#ifndef AUDIOMENU_HPP
#define AUDIOMENU_HPP

#include "scenes/BaseMenu.hpp"

class AudioMenu: public BaseMenu
{
public:
    AudioMenu();

private:
    void EventCallback(int id) override;

    gui::CheckBox* cb_music_;
    gui::Slider* sl_music_vol_;
    gui::CheckBox* cb_sound_;
    gui::Slider* sl_sound_vol_;
};

#endif // AUDIOMENU_HPP

