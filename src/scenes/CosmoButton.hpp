#ifndef COSMOBUTTON_HPP
#define COSMOBUTTON_HPP

#include "gui/Button.hpp"

/**
 * Sprite-based button for menus
 */
class CosmoButton: public gui::Button
{
public:
	CosmoButton(gui::Menu* owner, const sf::String& text);

protected:
	void OnStateChanged(gui::State::EState state) override;

	void OnCallbackTriggered() override;

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	sf::Sprite m_background;
};

#endif // COSMOBUTTON_HPP

