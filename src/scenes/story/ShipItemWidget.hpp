#ifndef SHIPITEMWIDGET_HPP
#define SHIPITEMWIDGET_HPP

#include "gui/Widget.hpp"
#include "items/ItemData.hpp"

class ShipItemWidget: public gui::Widget
{
public:
	ShipItemWidget(gui::Menu* parent, ItemData::Type type, const sf::Font& font);

	/**
	 * Refresh the item informations displayed in the UI
	 */
	void refresh();

	// callbacks ---------------------------------------------------------------

	void OnStateChanged(gui::State::EState state) override;
	void OnKeyPressed(sf::Keyboard::Key code) override;
	void OnMouseClicked(int x, int y) override;

private:
	bool buyNextLevel() const;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	ItemData::Type m_type;
	int            m_level;

	sf::Text   m_txt_name;
	sf::Text   m_txt_level;
	sf::Text   m_txt_description;
	sf::Text   m_txt_price;
	sf::Text   m_txt_upgrade;
	sf::Sprite m_background;
};

#endif // SHIPITEMWIDGET_HPP
