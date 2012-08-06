#ifndef GUI_TEXTBOX_HPP
#define GUI_TEXTBOX_HPP

#include "Widget.hpp"
#include "BitmapString.hpp"

namespace gui
{

/**
 * Un champs de saisie de texte
 */
class TextBox: public Widget
{
public:
	/**
	 * @param x: position x
	 * @param y: position y
	 * @param visible_chars: nombre de caratères visibles en largeur
	 * @param max_length: taille maximum de la saisie (-1 : pas de limite)
	 */
	TextBox(Menu* owner, int x, int y, int visible_chars = 10,
		int max_length = -1);

	void setString(const sf::String& text);

	const std::string& GetText() const;

	// inherited
	void Update(float frametime);

	// inherited callbacks
	void OnTextEntered(sf::Uint32 unicode);
	void OnKeyPressed(sf::Keyboard::Key code);
	void OnMouseClicked(int x, int y);

protected:
	// inherited
	void OnStateChanged(State::EState state);

private:
	// inherited
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	/**
	 * Positionne le curseur
	 * @param position: position du curseur dans le texte
	 */
	void SetCursor(int position);

	/**
	 * Obtenir la position du curseur dans le texte complet (au lieu de celui
	 * restreint à la box)
	 */
	int GetRealCursorPosition() const;

	/**
	 * Remplir la box avec les caractères qui dépassent, s'il reste de la place
	 */
	void FillBox();

	void ShiftLeft();

	void ShiftRight();

	//void Debug() const;

	sf::RectangleShape box_;
	BitmapString display_text_;
	std::string text_;
	sf::RectangleShape cursor_;
	// position du curseur
	int cursor_pos_;
	float cursor_timer_;
	// nombre de caractères visible (équivalent à la longueur de la box)
	int visible_chars_;
	// taille maximum de la saisie
	int max_length_;
	// nombre de caractères hors de la box à gauche
	int left_offset_;
	// à idem à droite
	int right_offset_;
};

}

#endif // GUI_TEXTBOX_HPP
