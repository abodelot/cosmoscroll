#ifndef JOYSTICKMENU_HPP
#define JOYSTICKMENU_HPP

#include "scenes/BaseMenu.hpp"
#include "scenes/ConfigButton.hpp"
#include "core/Input.hpp"

/**
 * Joystick layout configuration menu
 */
class JoystickMenu: public BaseMenu
{
public:
    JoystickMenu();

    void onEvent(const sf::Event& event) override;

    void onFocus() override;

private:
    ConfigButton* addRow(gui::FormLayout& form, Action::ID action);

    ConfigButton* m_triggered;
    ConfigButton* but_weapon_;
    ConfigButton* but_missile_;
    ConfigButton* but_cooler_;
    ConfigButton* but_pause_;
};

#endif // JOYSTICKMENU_HPP
