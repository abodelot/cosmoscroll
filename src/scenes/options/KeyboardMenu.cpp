#include "KeyboardMenu.hpp"
#include "core/Game.hpp"
#include "utils/I18n.hpp"


KeyboardMenu::KeyboardMenu():
	m_triggered(NULL)
{
	SetTitle(_t("menu.keyboard.title"));

	gui::FormLayout form(80, 120);
	form.SetSpacing(60, 10);
	but_up_      = addRow(form, Action::UP);
	but_down_    = addRow(form, Action::DOWN);
	but_left_    = addRow(form, Action::LEFT);
	but_right_   = addRow(form, Action::RIGHT);
	but_weapon_  = addRow(form, Action::USE_LASER);
	but_cooler_  = addRow(form, Action::USE_COOLER);
	but_missile_ = addRow(form, Action::USE_MISSILE);

	gui::Button* back = new CosmoButton(this, _t("menu.back"));
	back->setPosition(210, 410);
	back->SetCallbackID(9000);
}


void KeyboardMenu::OnEvent(const sf::Event& event)
{
	if (m_triggered != NULL)
	{
		// By-pass GUI event handler
		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code != sf::Keyboard::Escape)
			{
				// Bind action to pressed key
				Action::ID action = m_triggered->getAction();
				Input::setKeyBinding(event.key.code, action);
				OnFocus();
			}
			else
			{
				// Cancel input and reset button label
				m_triggered->setKeyboardLabel();
				m_triggered = NULL;
			}
		}
	}
	else
	{
		gui::Menu::OnEvent(event);
	}
}


void KeyboardMenu::OnFocus()
{
	m_triggered = NULL;
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


void KeyboardMenu::EventCallback(int id)
{
	// Input::Action enumerations are used as widget ids
	switch (id)
	{
		case Action::UP:          m_triggered = but_up_;      break;
		case Action::DOWN:        m_triggered = but_down_;    break;
		case Action::LEFT:        m_triggered = but_left_;    break;
		case Action::RIGHT:       m_triggered = but_right_;   break;
		case Action::USE_LASER:   m_triggered = but_weapon_;  break;
		case Action::USE_COOLER:  m_triggered = but_cooler_;  break;
		case Action::USE_MISSILE: m_triggered = but_missile_; break;
		case 9000:
			Game::getInstance().setNextScene(Game::SC_OptionMenu);
			break;
	}
	// We are now waiting for user input
	if (m_triggered != NULL)
	{
		m_triggered->setString(". . .");
	}
}


ConfigButton* KeyboardMenu::addRow(gui::FormLayout& form, Action::ID action)
{
	ConfigButton* button = new ConfigButton(this, action);
	button->setKeyboardLabel();
	form.AddRow(Action::toString(action), button);
	return button;
}
