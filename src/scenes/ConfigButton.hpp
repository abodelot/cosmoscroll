#ifndef CONFIGBUTTON_HPP
#define CONFIGBUTTON_HPP

#include "gui/Button.hpp"

/**
 * Small sprite-based button for key-bindings menus
 */
class ConfigButton: public gui::Button
{
public:
	ConfigButton(gui::Menu* owner, const sf::String& text);

protected:
	void OnStateChanged(gui::State::EState state) override;

	void OnCallbackTriggered() override;

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	static const sf::Texture* img_;
	xsf::Sprite background_;
};


#endif // CONFIGBUTTON_HPP

