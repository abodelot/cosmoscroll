#ifndef CONFIGBUTTON_HPP
#define CONFIGBUTTON_HPP

#include "gui/Button.hpp"


class ConfigButton: public gui::Button
{
public:
	ConfigButton(gui::Menu* owner, const sf::Unicode::Text& text);

protected:
	// override
	void OnStateChanged(gui::State::EState state);
	// override
	void OnCallbackTriggered();

private:
	// override
	void Render(sf::RenderTarget& target) const;

	static const sf::Image* img_;
	sf::Sprite background_;
};


#endif // CONFIGBUTTON_HPP

