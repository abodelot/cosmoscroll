#include "CreditCounterWidget.hpp"
#include "gui/Menu.hpp"
#include "utils/I18n.hpp"
#include "utils/SFML_Helper.hpp"
#include "core/Resources.hpp"

CreditCounterWidget::CreditCounterWidget(gui::Menu* parent):
	gui::Widget(parent, false)
{
	m_background.setTexture(Resources::getTexture("gui/credit-counter.png"));

	m_credit_label.setFont(*parent->GetWidgetStyle().global_font);
	m_credit_label.setCharacterSize(18);
	m_credit_label.setString(_t("levels.credits"));
	m_credit_label.setPosition((sfh::width(m_background) - sfh::width(m_credit_label)) / 2, 10);

	m_credit_value.setFont(*parent->GetWidgetStyle().global_font);
	m_credit_value.setCharacterSize(20);


	setPosition(444, 90);
}


void CreditCounterWidget::setCredits(int credits)
{
	m_credit_value.setString(std::to_string(credits));
	m_credit_value.setPosition((sfh::width(m_background) - sfh::width(m_credit_value)) / 2, 26);
}


void CreditCounterWidget::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(m_background, states);
	target.draw(m_credit_label, states);
	target.draw(m_credit_value, states);
}
