#include "ConfigButton.hpp"
#include "core/Resources.hpp"

const int       BUTTON_WIDTH  = 120;
const int       BUTTON_HEIGHT = 25;
const sf::Color ERROR_COLOR   = sf::Color(255, 220, 0);


ConfigButton::ConfigButton(gui::Menu* owner, Action::ID action):
	gui::Button(owner, "", BUTTON_WIDTH, BUTTON_HEIGHT),
	m_action(action),
	m_error(false)
{
	m_background.setTexture(Resources::getTexture("gui/button-config.png"));
	m_background.setTextureRect(sf::IntRect(0, 0, BUTTON_WIDTH, BUTTON_HEIGHT));
	SetCallbackID(action);
	OnStateChanged(GetState());
}


void ConfigButton::setKeyboardLabel()
{
	sf::Keyboard::Key key = Input::getKeyBinding(m_action);
	setString(Input::keyToString(key));

	// Check for error and refresh label
	m_error = key == sf::Keyboard::Unknown;
	OnStateChanged(GetState());
}


void ConfigButton::setJoystickLabel()
{
	unsigned int button = Input::getButtonBinding(m_action);
	setString(Input::buttonToString(button));

	// Check for error and refresh label
	m_error = button == sf::Joystick::ButtonCount;
	OnStateChanged(GetState());
}


Action::ID ConfigButton::getAction() const
{
	return m_action;
}


void ConfigButton::OnStateChanged(gui::State::EState state)
{
	gui::Button::OnStateChanged(state);
	switch (state)
	{
		case gui::State::DEFAULT:
			m_background.setTextureRect(sf::IntRect(0, 0, BUTTON_WIDTH, BUTTON_HEIGHT));
			break;
		case gui::State::HOVERED:
			m_background.setTextureRect(sf::IntRect(0, BUTTON_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT));
			break;
		case gui::State::FOCUSED:
			m_background.setTextureRect(sf::IntRect(0, BUTTON_HEIGHT * 2, BUTTON_WIDTH, BUTTON_HEIGHT));
			break;
		default:
			break;
	}

	if (m_error)
	{
		// Override label color when binding is invalid
		setColor(ERROR_COLOR);
	}
}


void ConfigButton::OnCallbackTriggered()
{
}


void ConfigButton::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(m_background, states);
	target.draw((gui::Button) *this);
}

