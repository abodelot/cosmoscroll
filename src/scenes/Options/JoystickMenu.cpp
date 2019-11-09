#include "JoystickMenu.hpp"
#include "core/Game.hpp"
#include "utils/I18n.hpp"


JoystickMenu::JoystickMenu():
    m_triggered(nullptr)
{
    setTitle(_t("joystick.title"));

    gui::FormLayout form(110, 120);
    form.SetSpacing(20, 16);
    but_weapon_  = addRow(form, Action::USE_LASER);
    but_missile_ = addRow(form, Action::USE_MISSILE);
    but_cooler_  = addRow(form, Action::USE_COOLER);
    but_pause_   = addRow(form, Action::PAUSE);

    gui::Button* back = new CosmoButton(this, _t("back"));
    back->setPosition(210, 410);
    back->setCallback([]() {
        Game::getInstance().setCurrentScreen(Game::SC_OptionMenu);
    });
}


void JoystickMenu::onEvent(const sf::Event& event)
{
    if (m_triggered != nullptr)
    {
        // Waiting for user input: bypass GUI event handler
        if (event.type == sf::Event::JoystickButtonPressed)
        {
            // Binding action to pressed button
            Action::ID action = m_triggered->getAction();
            Input::setButtonBinding(event.joystickButton.button, action);
            onFocus();
        }
        else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
        {
            // Cancel input and reset button label
            m_triggered->setJoystickLabel();
            m_triggered = nullptr;
        }
    }
    else
    {
        gui::Menu::OnEvent(event);
    }
}


void JoystickMenu::onFocus()
{
    m_triggered = nullptr;
    // Refresh labels
    but_weapon_->setJoystickLabel();
    but_missile_->setJoystickLabel();
    but_cooler_->setJoystickLabel();
    but_pause_->setJoystickLabel();
}


ConfigButton* JoystickMenu::addRow(gui::FormLayout& form, Action::ID action)
{
    ConfigButton* button = new ConfigButton(this, action);
    button->setJoystickLabel();
    button->setCallback([button, this]() {
        m_triggered = button;
        // We are now waiting for user input
        m_triggered->setString("<press button>");
    });
    form.addRow(Action::toString(action), button);
    return button;
}
