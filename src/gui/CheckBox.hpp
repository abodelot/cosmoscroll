#ifndef GUI_CHECKBOX_HPP
#define GUI_CHECKBOX_HPP

#include "Widget.hpp"

namespace gui
{

/**
 * Une case à cocher pour un choix binaire
 * callback appelée quand : choix modifié
 */
class CheckBox: public Widget {
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

    void onKeyPressed(sf::Keyboard::Key code) override;
    void onMouseClicked(int, int) override;

protected:
    void onStateChanged(State::EState state) override;

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    bool checked_;
    sf::RectangleShape box_;
    sf::RectangleShape v1_;
    sf::RectangleShape v2_;
};

}

#endif // GUI_CHECKBOX_HPP

