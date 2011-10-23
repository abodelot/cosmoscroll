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

	void SetPosition(float x, float y);
	void SetPosition(const sf::Vector2f& position);

	/**
	 * Changer l'état du widget (affichage uniqement)
	 */
	void SetState(State::EState state);
	State::EState GetState() const;

	inline void SetVisible(bool visible) { visible_ = visible; }
	inline bool IsVisible() const { return visible_; }

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

protected:
	void SetRect(int x, int y, int x2, int y2);
	void SetRect(const sf::FloatRect& rect);
	void Resize(int width, int height);

	Menu* GetOwner() const;

	void CallTheCallback();

	/**
	 * Callback état modifié
	 * @param state: nouvel état du widget
	 */
	virtual void OnStateChanged(State::EState) {}

	/**
	 * Callback si callback déclenchée
	 */
	virtual void OnCallbackTriggered() {}

private:
	Menu* owner_;
	int callback_id_;
	sf::FloatRect rect_;
	bool focusable_;
	bool visible_;
	State::EState state_;
};

}

#endif // GUI_WIDGET_HPP
