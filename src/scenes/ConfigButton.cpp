#include "ConfigButton.hpp"
#include "core/Resources.hpp"

#define BUT_W 120
#define BUT_H 25


ConfigButton::ConfigButton(gui::Menu* owner, Action::ID action):
	gui::Button(owner, "", BUT_W, BUT_H),
	m_action(action)
{
	m_background.setTexture(Resources::getTexture("gui/button-config.png"));
	m_background.setTextureRect(sf::IntRect(0, 0, BUT_W, BUT_H));
	SetTextPadding(0, -2);
	SetAlign(gui::Align::CENTER);
	SetCallbackID(action);
	OnStateChanged(GetState());
}


void ConfigButton::setKeyboardLabel()
{
	setString(Input::keyToString(Input::getKeyBinding(m_action)));
}


void ConfigButton::setJoystickLabel()
{
	setString(Input::buttonToString(Input::getButtonBinding(m_action)));
}


Action::ID ConfigButton::getAction() const
{
	return m_action;
}


void ConfigButton::OnStateChanged(gui::State::EState state)
{
	switch (state)
	{
		case gui::State::DEFAULT:
			m_background.setTextureRect(sf::IntRect(0, 0, BUT_W, BUT_H));
			break;
		case gui::State::HOVERED:
			m_background.setTextureRect(sf::IntRect(0, BUT_H, BUT_W, BUT_H));
			break;
		case gui::State::FOCUSED:
			m_background.setTextureRect(sf::IntRect(0, BUT_H * 2, BUT_W, BUT_H));
			break;
		default:
			break;
	}
	gui::Button::OnStateChanged(state);
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

