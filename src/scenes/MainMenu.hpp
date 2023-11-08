#ifndef MAINMENU_HPP
#define MAINMENU_HPP

#include "BaseMenu.hpp"

/**
 * Application main menu
 */
class MainMenu: public BaseMenu {
public:
    MainMenu();

    void draw(sf::RenderTarget& target) const override;

private:
    sf::Sprite m_title;
};

#endif // MAINMENU_HPP
