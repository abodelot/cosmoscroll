#include "JoystickMenu.hpp"
#include "core/Game.hpp"
#include "utils/I18n.hpp"


JoystickMenu::JoystickMenu():
	m_triggered(NULL)
{
	SetTitle(_t("menu.joystick.title"));

	gui::FormLayout form(110, 120);
	form.SetSpacing(20, 16);
	but_weapon_  = addRow(form, Action::USE_LASER);
	but_missile_ = addRow(form, Action::USE_MISSILE);
	but_cooler_  = addRow(form, Action::USE_COOLER);
	but_pause_   = addRow(form, Action::PAUSE);

	sl_joystick_ = new gui::Slider(this, 160);
	sl_joystick_->SetCallbackID(9000);
	form.AddRow(_t("menu.joystick.sensitivity"), sl_joystick_);

	gui::Button* back = new CosmoButton(this, _t("menu.back"));
	back->setPosition(210, 410);
	back->SetCallbackID(9001);
}


void JoystickMenu::OnEvent(const sf::Event& event)
{
	if (m_triggered != NULL)
	{
		// By-pass GUI event handler
		if (event.type == sf::Event::JoystickButtonPressed)
		{
			// Binding action to pressed button
			Action::ID action = m_triggered->getAction();
			Input::setButtonBinding(event.joystickButton.button, action);
			OnFocus();
		}
		else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
		{
			// Cancel input and reset button label
			m_triggered->setJoystickLabel();
			m_triggered = NULL;
		}
	}
	else
	{
		gui::Menu::OnEvent(event);
	}
}


void JoystickMenu::OnFocus()
{
	m_triggered = NULL;
	// Refresh labels
	but_weapon_->setJoystickLabel();
	but_missile_->setJoystickLabel();
	but_cooler_->setJoystickLabel();
	but_pause_->setJoystickLabel();

	sl_joystick_->SetValue(100 - Input::getJoystickDeadzone());
}


void JoystickMenu::EventCallback(int id)
{
	// Input::Action enumerations are used as widget ids
	switch (id)
	{
		case Action::USE_LASER:   m_triggered = but_weapon_;  break;
		case Action::USE_COOLER:  m_triggered = but_cooler_;  break;
		case Action::USE_MISSILE: m_triggered = but_missile_; break;
		case Action::PAUSE:       m_triggered = but_pause_;   break;
		case 9000:
			Input::setJoystickDeadzone(100 - sl_joystick_->GetValue());
			break;
		case 9001:
			Game::getInstance().setNextScene(Game::SC_OptionMenu);
			break;
	}
	// We are now waiting for user input
	if (m_triggered != NULL)
	{
		m_triggered->setString(". . .");
	}
}


ConfigButton* JoystickMenu::addRow(gui::FormLayout& form, Action::ID action)
{
	ConfigButton* button = new ConfigButton(this, action);
	button->setJoystickLabel();
	form.AddRow(Action::toString(action), button);
	return button;
}
