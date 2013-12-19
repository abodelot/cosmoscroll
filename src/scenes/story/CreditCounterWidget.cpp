#include "CreditCounterWidget.hpp"
#include "gui/Menu.hpp"
#include "utils/I18n.hpp"
#include "core/Resources.hpp"

CreditCounterWidget::CreditCounterWidget(gui::Menu* parent):
	gui::Widget(parent, false)
{
	m_background.setTexture(Resources::getTexture("gui/credit-counter.png"));

	m_credits.setFont(*parent->GetWidgetStyle().global_font);
	m_credits.setCharacterSize(18);
	m_credits.setPosition(16, 10);

	setPosition(444, 90);
}


void CreditCounterWidget::setCredits(int credits)
{
	m_credits.setString(I18n::templatize("menu.story.credits", "{credits}", credits));
}


void CreditCounterWidget::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(m_background, states);
	target.draw(m_credits, states);
}
