#include "CreditCounterWidget.hpp"
#include "gui/Menu.hpp"
#include "core/Services.hpp"
#include "core/Resources.hpp"
#include "utils/SFML_Helper.hpp"

CreditCounterWidget::CreditCounterWidget(gui::Menu* parent):
    gui::Widget(parent, false)
{
    m_background.setTexture(Resources::getTexture("gui/credit-counter.png"));

    m_creditLabel.setFont(*parent->getWidgetStyle().global_font);
    m_creditLabel.setCharacterSize(14);
    m_creditLabel.setString(_t("levels.credits"));
    m_creditLabel.setPosition((int) (sfh::width(m_background) - sfh::width(m_creditLabel)) / 2, 10);
    m_creditLabel.setOutlineColor(sf::Color(0, 0, 0, 128));
    m_creditLabel.setOutlineThickness(1.f);

    m_creditValue.setFont(*parent->getWidgetStyle().global_font);
    m_creditValue.setCharacterSize(18);
    m_creditValue.setOutlineColor(sf::Color(0, 0, 0, 128));
    m_creditValue.setOutlineThickness(1.f);

    setPosition(444, 90);
}


void CreditCounterWidget::setCredits(int credits)
{
    m_creditValue.setString(std::to_string(credits));
    m_creditValue.setPosition((int) (sfh::width(m_background) - sfh::width(m_creditValue)) / 2, 26);
}


void CreditCounterWidget::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(m_background, states);
    target.draw(m_creditLabel, states);
    target.draw(m_creditValue, states);
}
