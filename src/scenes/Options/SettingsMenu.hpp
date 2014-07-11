#ifndef SETTINGSMENU_HPP
#define SETTINGSMENU_HPP

#include "scenes/BaseMenu.hpp"

class SettingsMenu: public BaseMenu
{
public:
	SettingsMenu();

private:
	void EventCallback(int id) override;

	void addResolution(size_t width, size_t height);

	gui::CheckBox*                 m_vsync;
	gui::OptionList<sf::Vector2u>* m_resolutions;
	gui::OptionList<std::string>*  m_languages;
	gui::Button*                   m_back;
	gui::FormLayout                m_form;
};

#endif // SETTINGSMENU_HPP

