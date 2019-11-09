#include "KeyboardMenu.hpp"
#include "core/Game.hpp"
#include "utils/I18n.hpp"


KeyboardMenu::KeyboardMenu():
    m_triggered(nullptr)
{
    setTitle(_t("keyboard.title"));

    gui::FormLayout form(140, 120);
    form.SetSpacing(20, 10);
    but_up_      = addRow(form, Action::UP);
    but_down_    = addRow(form, Action::DOWN);
    but_left_    = addRow(form, Action::LEFT);
    but_right_   = addRow(form, Action::RIGHT);
    but_weapon_  = addRow(form, Action::USE_LASER);
    but_cooler_  = addRow(form, Action::USE_COOLER);
    but_missile_ = addRow(form, Action::USE_MISSILE);

    gui::Button* back = new CosmoButton(this, _t("back"));
    back->setPosition(210, 410);
    back->setCallback([]() {
        Game::getInstance().setCurrentScreen(Game::SC_OptionMenu);
    });
}


void KeyboardMenu::onEvent(const sf::Event& event)
{
    if (m_triggered != nullptr)
    {
        // Waiting for user input: bypass GUI event handler
        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code != sf::Keyboard::Escape)
            {
                // Bind action to pressed key
                Action::ID action = m_triggered->getAction();
                Input::setKeyBinding(event.key.code, action);
                onFocus();
            }
            else
            {
                // Cancel input and reset button label
                m_triggered->setKeyboardLabel();
                m_triggered = nullptr;
            }
        }
    }
    else
    {
        gui::Menu::OnEvent(event);
    }
}


void KeyboardMenu::onFocus()
{
    m_triggered = nullptr;
    // Refresh labels
    but_up_->setKeyboardLabel();
    but_up_->setKeyboardLabel();
    but_down_->setKeyboardLabel();
    but_left_->setKeyboardLabel();
    but_right_->setKeyboardLabel();
    but_weapon_->setKeyboardLabel();
    but_cooler_->setKeyboardLabel();
    but_missile_->setKeyboardLabel();
}


ConfigButton* KeyboardMenu::addRow(gui::FormLayout& form, Action::ID action)
{
    ConfigButton* button = new ConfigButton(this, action);
    button->setKeyboardLabel();
    button->setCallback([button, this]() {
        m_triggered = button;
        // We are now waiting for user input
        m_triggered->setString("<press key>");
    });
    form.addRow(Action::toString(action), button);
    return button;
}
