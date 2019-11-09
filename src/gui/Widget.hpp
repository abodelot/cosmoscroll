#ifndef GUI_WIDGET_HPP
#define GUI_WIDGET_HPP

#include <SFML/Graphics.hpp>
#include <functional>

namespace gui
{

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
 * Abstract base class for GUI elements
 */
class Widget: public sf::Drawable, public sf::Transformable
{
public:
    static const int MIN_HEIGHT = 25;

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

    void setCallback(std::function<void(void)> callback);

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
    void triggerCallback();

    /**
     * Callback état modifié
     * @param state: nouvel état du widget
     */
    virtual void OnStateChanged(State::EState) {}

    /**
     * Callback si callback déclenchée
     */
    virtual void OnCallbackTriggered() {}

    void centerText(sf::Text& text);

private:
    Menu* owner_;
    std::function<void(void)> callback_;
    int width_;
    int height_;
    bool focusable_;
    bool visible_;
    State::EState state_;
};

}

#endif // GUI_WIDGET_HPP
