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
    void setState(State::EState state);
    State::EState getState() const;

    inline void setVisible(bool visible) { visible_ = visible; }
    inline bool isVisible() const { return visible_; }

    void setCallback(std::function<void(void)> callback);

    /**
     * Mise à jour du widget
     * @param frametime: temps de la frame courante
     */
    virtual void update(float frametime);

    // callbacks des événements -----------------------------------------------

    virtual void onKeyPressed(sf::Keyboard::Key) {}

    virtual void onTextEntered(sf::Uint32) {}

    virtual void onMouseClicked(int, int) {}

    virtual void onMouseWheelMoved(int) {}

    /**
     * Holds widget dimension
     */
    void resize(int width, int height);
    int getWidth() const;
    int getHeight() const;

    /**
     * @return true si le widget peut prendre le focus, sinon false
     */
    bool canGrabFocus() const;

    bool isFocused() const;

    Menu* getOwner() const;

    bool containsPoint(float x, float y);

protected:
    void triggerCallback();

    /**
     * Callback état modifié
     * @param state: nouvel état du widget
     */
    virtual void onStateChanged(State::EState) {}

    /**
     * Callback si callback déclenchée
     */
    virtual void onCallbackTriggered() {}

    void centerText(sf::Text& text);

private:
    Menu* owner_;
    std::function<void(void)> callback_;
    int width_;
    int height_;
    bool focusable_;
    bool visible_;
    State::EState m_state;
};

}

#endif // GUI_WIDGET_HPP
