#ifndef GUI_SLIDER_HPP
#define GUI_SLIDER_HPP

#include "Widget.hpp"

namespace gui
{

/**
 * Callback appelée quand : valeur modifiée
 */
class Slider: public Widget
{
public:
	/**
	 * @param owner: menu propriétaire du slider
	 * @param w: largeur
	 * @param h: hauteur (-1 => taille de texte du menu)
	 */
	Slider(Menu* owner, int w, int h=-1);

	/**
	 * Indiquer le pas de variation de la poignée
	 */
	void SetQuantum(int quantum);

	int GetValue() const;
	void SetValue(int value);

	// inherited callbacks
	void OnKeyPressed(sf::Keyboard::Key code);
	void OnMouseClicked(int x, int y);
	void OnMouseWheelMoved(int delta);

protected:
	// inherited
	void OnStateChanged(State::EState state);

private:
	// inherited
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	void UpdateHandle(int value);

	sf::RectangleShape bar_;
	sf::RectangleShape handle_;
	int handle_index_;
	int quantum_;
};

}

#endif // GUI_SLIDER_HPP
