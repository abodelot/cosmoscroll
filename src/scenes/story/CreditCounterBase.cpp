#include "CreditCounterBase.hpp"
#include "utils/I18n.hpp"
#include "utils/StringUtils.hpp"
#include "core/PlayerSave.hpp"

CreditCounterBase::CreditCounterBase()
{
	credit_counter_bg_.setPosition(444, 90);
	credit_counter_bg_.setTexture(Resources::getTexture("gui/credit-counter.png"));

	credit_counter_.setPosition(460, 100);
	credit_counter_.setFont(GetMenuFont());
	credit_counter_.setCharacterSize(18);
}


void CreditCounterBase::OnFocus()
{
	BaseMenu::OnFocus();

	int credits = PlayerSave::getCredits();
	credit_counter_.setString(I18n::templatize("menu.story.credits", "{credits}", credits));
}


void CreditCounterBase::Show(sf::RenderTarget& target) const
{
	BaseMenu::Show(target);
	target.draw(credit_counter_bg_);
	target.draw(credit_counter_);
}
