#include "ConfigButton.hpp"
#include "core/Resources.hpp"

#define BUT_W 120
#define BUT_H 25

const sf::Texture* ConfigButton::img_ = NULL;

ConfigButton::ConfigButton(gui::Menu* owner, const sf::String& text) :
	gui::Button(owner, text, BUT_W, BUT_H)
{
	if (img_ == NULL)
	{
		img_ = &Resources::getTexture("gui/button-config.png");
	}

	background_.setTexture(*img_);
	background_.setTextureRect(sf::IntRect(0, 0, BUT_W, BUT_H));
	background_.resize(BUT_W, BUT_H);

	SetTextPadding(0, -2);
	SetAlign(gui::Align::CENTER);
	OnStateChanged(GetState());
}


void ConfigButton::OnStateChanged(gui::State::EState state)
{
	switch (state)
	{
		case gui::State::DEFAULT:
			background_.setTextureRect(sf::IntRect(0, 0, BUT_W, BUT_H));
			break;
		case gui::State::HOVERED:
			background_.setTextureRect(sf::IntRect(0, BUT_H, BUT_W, BUT_H));
			break;
		case gui::State::FOCUSED:
			background_.setTextureRect(sf::IntRect(0, BUT_H * 2, BUT_W, BUT_H));
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
	target.draw(background_, states);
	target.draw((gui::Button) *this);
}

