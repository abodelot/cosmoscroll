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
	 * @param w: largeur (-1 pour largeur automatiquement adaptée au texte)
	 * @param h: hauteur (-1 pour hauteur automatiquement adaptée au texte)
	 */
	Button(Menu* owner, const sf::String& text, int w = -1, int h = -1);

	/**
	 * Indiquer le texte affiché sur le bouton
	 */
	virtual void setString(const sf::String& text);

	void SetTextPadding(int x, int y);

	/**
	 * Indiquer l'alignement du texte
	 */
	virtual void SetAlign(Align::EAlign align);

	// callbacks
	virtual void OnKeyPressed(sf::Keyboard::Key code);
	virtual void OnMouseClicked(int, int);

protected:
	// override
	virtual void OnStateChanged(State::EState state);

	// override
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	sf::Text text_;
	Align::EAlign align_;
};

}

#endif // GUI_BUTTON_HPP
