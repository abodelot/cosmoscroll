#ifndef GUI_WIDGET_HPP
#define GUI_WIDGET_HPP

#include <SFML/Graphics.hpp>

namespace gui
{

namespace Align
{
	enum EAlign
	{
		LEFT,
		RIGHT,
		CENTER
	};
}

namespace State
{
	enum EState
	{
		DEFAULT,
		HOVERED,
		FOCUSED,
		HIDDEN,
		DISABLED // TODO
	};
}

class Menu;

/**
 * Base de tous les éléments graphiques (classe abstraite)
 */
class Widget: public sf::Drawable
{
public:
	/**
	 * @param owner: menu conteneur du widget
	 * @param focsable: indique si le widget peut prendre le focus
	 */
	Widget(Menu* owner, bool focusable);

	/**
	 * Changer l'état du widget (affichage uniqement)
	 */
	void SetState(State::EState state);
	State::EState GetState() const;

	/**
	 * Mise à jour du widget
	 * @param frametime: temps de la frame courante
	 */
	virtual void Update(float frametime);

	// callbacks des événements -----------------------------------------------

	virtual void OnKeyPressed(sf::Key::Code) {}

	virtual void OnTextEntered(sf::Uint32) {}

	virtual void OnMouseClicked(int, int) {}

	virtual void OnMouseWheelMoved(int) {}

	void SetCallbackID(int id)
	{
		callback_id_ = id;
	}

	/**
	 * Obtenir la surface du widget
	 * @return rectangle occupé par la surface
	 */
	const sf::FloatRect& GetRect() const;
	int GetWidth() const;
	int GetHeight() const;

	/**
	 * @return true si le widget peut prendre le focus, sinon false
	 */
	bool CanGrabFocus() const;

	bool IsFocused() const;

	struct WidgetStyle
	{
		// global
		int global_text_size;
		const sf::Font* global_font;
		sf::Color global_border_color;

		// label
		sf::Color label_text_color;

		// button
		sf::Color button_text_color;
		sf::Color button_text_color_focus;

		// checkbox
		sf::Color ckbox_v_color;
		sf::Color ckbox_v_color_focus;
		sf::Color ckbox_bg_color;
		sf::Color ckbox_bg_color_focus;
		int ckbox_size;

		// slider
		sf::Color slider_handle_color;
		sf::Color slider_handle_color_focus;
		sf::Color slider_bg_color;
		sf::Color slider_bg_color_focus;

		// optionlist
		sf::Color optlist_arrow_color;
		sf::Color optlist_arrow_color_focus;
		sf::Color optlist_bg_color;
		sf::Color optlist_bg_color_focus;

		// textbox
		sf::Color textbox_bg_color;
		sf::Color textbox_bg_color_focus;
	};


protected:
	void SetRect(int x, int y, int x2, int y2);
	void SetRect(const sf::FloatRect& rect);

	Menu* GetOwner() const;

	void CallTheCallback();

	/**
	 * Callback état modifié
	 * @param state: nouvel état du widget
	 */
	virtual void OnStateChanged(State::EState state) {}

	/**
	 * Callback si callback déclenchée
	 */
	virtual void OnCallbackTriggered() {}

private:
	Menu* owner_;
	int callback_id_;
	sf::FloatRect rect_;
	bool focusable_;
	State::EState state_;
};

}

#endif // GUI_WIDGET_HPP
