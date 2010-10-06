#ifndef COSMOBUTTON_HPP
#define COSMOBUTTON_HPP

#include "../gui/Button.hpp"

class CosmoButton: public gui::Button
{

public:
	CosmoButton(gui::Menu* owner, const sf::Unicode::Text& text, int x, int y);

	static void SetBackgroundImage(const sf::Image* img);
protected:
	// inherited
	void OnStateChanged(gui::State::EState state);

private:
	// inherited
	void Render(sf::RenderTarget& target) const;

	static const sf::Image* img_;

	sf::Sprite background_;
};

#endif // COSMOBUTTON_HPP

