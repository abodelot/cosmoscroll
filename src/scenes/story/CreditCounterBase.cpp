#include "CreditCounterBase.hpp"
#include "utils/I18n.hpp"
#include "utils/StringUtils.hpp"
#include "core/Game.hpp"

CreditCounterBase::CreditCounterBase()
{
	credit_counter_bg_.SetPosition(444, 90);
	credit_counter_bg_.SetImage(Resources::GetImage("gui/credit-counter.png"));

	credit_counter_.SetPosition(460, 100);
	credit_counter_.SetFont(GetMenuFont());
	credit_counter_.SetSize(18);
}


void CreditCounterBase::OnFocus()
{
	BaseMenu::OnFocus();

	int credits = Game::GetPlayerSave().GetCredits();
	credit_counter_.SetText(wstr_replace(_t("menu.story.credits"), L"{credits}", to_wstring(credits)));
}


void CreditCounterBase::Show(sf::RenderTarget& target) const
{
	BaseMenu::Show(target);
	target.Draw(credit_counter_bg_);
	target.Draw(credit_counter_);
}
