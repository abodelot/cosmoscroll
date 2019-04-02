#ifndef GUI_CHECKBOX_HPP
#define GUI_CHECKBOX_HPP

#include "Widget.hpp"

namespace gui
{

/**
 * Une case à cocher pour un choix binaire
 * callback appelée quand : choix modifié
 */
class CheckBox: public Widget
{
public:
    CheckBox(Menu* owner);

    /**
     * @return true si la case à cocher est cochée
     */
    bool Checked() const;

    /**
     * Check/uncheck the checkbox
     */
    void Check(bool checked);

    // inherited callbacks
    void OnKeyPressed(sf::Keyboard::Key code);
    void OnMouseClicked(int, int);

protected:
    // inherited
    void OnStateChanged(State::EState state);

private:
    // inherited
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    bool checked_;
    sf::RectangleShape box_;
    sf::RectangleShape v1_;
    sf::RectangleShape v2_;
};

}

#endif // GUI_CHECKBOX_HPP

