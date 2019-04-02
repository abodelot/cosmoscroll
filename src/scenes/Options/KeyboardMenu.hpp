#ifndef KEYBOARDMENU_HPP
#define KEYBOARDMENU_HPP

#include "scenes/BaseMenu.hpp"
#include "scenes/ConfigButton.hpp"
#include "core/Input.hpp"

/**
 * Keyboard layout configuration menu
 */
class KeyboardMenu: public BaseMenu
{
public:
    KeyboardMenu();

    void onEvent(const sf::Event& event) override;

    void onFocus() override;

private:
    void EventCallback(int id) override;

    ConfigButton* addRow(gui::FormLayout& form, Action::ID action);

    ConfigButton* m_triggered;
    ConfigButton* but_up_;
    ConfigButton* but_down_;
    ConfigButton* but_left_;
    ConfigButton* but_right_;
    ConfigButton* but_weapon_;
    ConfigButton* but_cooler_;
    ConfigButton* but_missile_;
};

#endif // KEYBOARDMENU_HPP

