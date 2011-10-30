#include "CosmoButton.hpp"
#include "core/SoundSystem.hpp"
#include "utils/Resources.hpp"

#define BUT_W 220
#define BUT_H 40

const sf::Image* CosmoButton::img_ = NULL;

CosmoButton::CosmoButton(gui::Menu* owner, const sf::Unicode::Text& text) :
	gui::Button(owner, text, BUT_W, BUT_H)
{
	if (img_ == NULL)
	{
		img_ = &Resources::GetImage("gui/button.png");
	}

	background_.SetImage(*img_);
	background_.SetSubRect(sf::IntRect(0, 0, BUT_W, BUT_H));
	background_.Resize(BUT_W, BUT_H);

	SetTextPadding(0, 8);
	SetAlign(gui::Align::CENTER);
	OnStateChanged(GetState());
}


void CosmoButton::OnStateChanged(gui::State::EState state)
{
	switch (state)
	{
		case gui::State::DEFAULT:
			background_.SetSubRect(sf::IntRect(0, 0, BUT_W, BUT_H));
			break;
		case gui::State::HOVERED:
			background_.SetSubRect(sf::IntRect(0, 40, BUT_W, 40 + BUT_H));
			SoundSystem::GetInstance().PlaySound(Resources::GetSoundBuffer("menu-select.wav"));
			break;
		case gui::State::FOCUSED:
			background_.SetSubRect(sf::IntRect(0, 80, BUT_W, 80 + BUT_H));
			break;
		default:
			break;
	}
	gui::Button::OnStateChanged(state);
}


void CosmoButton::OnCallbackTriggered()
{
	SoundSystem::GetInstance().PlaySound(Resources::GetSoundBuffer("menu-valid.ogg"));
}


void CosmoButton::Render(sf::RenderTarget& target) const
{
	target.Draw(background_);
	gui::Button::Render(target);
}
