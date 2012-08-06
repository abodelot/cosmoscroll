#ifndef COSMOBUTTON_HPP
#define COSMOBUTTON_HPP

#include "gui/Button.hpp"

/**
 * Bouton avec un style avancé (120px * 40px)
 */
class CosmoButton: public gui::Button
{
public:
	CosmoButton(gui::Menu* owner, const sf::String& text);

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

#endif // COSMOBUTTON_HPP

