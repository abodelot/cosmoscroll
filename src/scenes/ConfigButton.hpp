#ifndef CONFIGBUTTON_HPP
#define CONFIGBUTTON_HPP

#include "gui/Button.hpp"


class ConfigButton: public gui::Button
{
public:
	ConfigButton(gui::Menu* owner, const sf::String& text);

protected:
	// override
	void OnStateChanged(gui::State::EState state);
	// override
	void OnCallbackTriggered();

private:
	// override
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	static const sf::Texture* img_;
	xsf::Sprite background_;
};


#endif // CONFIGBUTTON_HPP

