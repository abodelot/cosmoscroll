#ifndef SHIPITEMWIDGET_HPP
#define SHIPITEMWIDGET_HPP

#include "gui/Widget.hpp"
#include "items/Item.hpp"

class ShipItemWidget: public gui::Widget
{
public:
    ShipItemWidget(gui::Menu* parent, Item::Type type, const sf::Font& font);

    /**
     * Refresh the item informations displayed in the UI
     */
    void refresh();

    void onStateChanged(gui::State::EState state) override;
    void onKeyPressed(sf::Keyboard::Key code) override;
    void onMouseClicked(int x, int y) override;

private:
    bool buyNextLevel() const;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    Item::Type m_type;
    int        m_level;

    sf::Text   m_txtName;
    sf::Text   m_txtLevel;
    sf::Text   m_txtDescription;
    sf::Text   m_txtPrice;
    sf::Text   m_txtUpgrade;
    sf::Sprite m_background;
};

#endif // SHIPITEMWIDGET_HPP
