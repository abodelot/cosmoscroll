#ifndef CREDITCOUNTERWIDGET_HPP
#define CREDITCOUNTERWIDGET_HPP

#include "gui/Widget.hpp"

/**
 * Widget for displaying player's credits
 */
class CreditCounterWidget: public gui::Widget
{
public:
	CreditCounterWidget(gui::Menu* parent);

	void setCredits(int credits);

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	sf::Sprite m_background;
	sf::Text   m_credit_label;
	sf::Text   m_credit_value;
};

#endif // CREDITCOUNTERWIDGET_HPP
