#ifndef GUI_BUTTON_HPP
#define GUI_BUTTON_HPP

#include "Widget.hpp"

namespace gui
{

/**
 * Un bouton cliquable
 * callback si : touche <Enter>
 */
class Button: public Widget
{
public:
	/**
	 * @param owner: menu propriétaire du bouton
	 * @param text: contenu affiché
	 * @param x: position x
	 * @param y: position y
	 * @param w: largeur (-1 pour largeur automatiquement adaptée au texte)
	 * @param h: hauteur (-1 pour hauteur automatiquement adaptée au texte)
	 */
	Button(Menu* owner, const sf::Unicode::Text& text, int x=0, int y=0, int w = -1, int h = -1);

	/**
	 * Indiquer le texte affiché sur le bouton
	 */
	virtual void SetText(const sf::Unicode::Text& text);

	void SetTextPadding(int x, int y);

	/**
	 * Indiquer l'alignement du texte
	 */
	virtual void SetAlign(Align::EAlign align);

	// callbacks
	virtual void OnKeyPressed(sf::Key::Code code);
	virtual void OnMouseClicked(int, int);

protected:
	// override
	virtual void OnStateChanged(State::EState state);

	// override
	virtual void Render(sf::RenderTarget& target) const;

private:
	sf::String text_;
	Align::EAlign align_;
};

}

#endif // GUI_BUTTON_HPP
