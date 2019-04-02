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
class Widget: public sf::Drawable, public sf::Transformable
{
public:
    /**
     * @param owner: menu conteneur du widget
     * @param focsable: indique si le widget peut prendre le focus
     */
    Widget(Menu* owner, bool focusable);

    /**
     * Holds widget state (for display purpose only)
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

    virtual void OnKeyPressed(sf::Keyboard::Key) {}

    virtual void OnTextEntered(sf::Uint32) {}

    virtual void OnMouseClicked(int, int) {}

    virtual void OnMouseWheelMoved(int) {}

    void SetCallbackID(int id)
    {
        callback_id_ = id;
    }

    /**
     * Holds widget dimension
     */
    void Resize(int width, int height);
    int GetWidth() const;
    int GetHeight() const;

    /**
     * @return true si le widget peut prendre le focus, sinon false
     */
    bool CanGrabFocus() const;

    bool IsFocused() const;

    Menu* GetOwner() const;

    bool ContainsPoint(float x, float y);

protected:
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

    void CenterText(sf::Text& text);

private:
    Menu* owner_;
    int callback_id_;
    int width_;
    int height_;
    bool focusable_;
    bool visible_;
    State::EState state_;
};

}

#endif // GUI_WIDGET_HPP
