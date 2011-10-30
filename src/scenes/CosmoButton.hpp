#ifndef COSMOBUTTON_HPP
#define COSMOBUTTON_HPP

#include "gui/Button.hpp"

/**
 * Bouton avec un style avancé (120px * 40px)
 */
class CosmoButton: public gui::Button
{
public:
	CosmoButton(gui::Menu* owner, const sf::Unicode::Text& text);

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

#endif // COSMOBUTTON_HPP

