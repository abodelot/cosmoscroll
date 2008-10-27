#ifndef WINDOW_HPP
#define WINDOW_HPP

// constantes de configuration de la fenêtre
#define WIN_WIDTH   640
#define WIN_HEIGHT  480
#define WIN_BPP     32
#define WIN_FPS     60
#define WIN_TITLE   "CosmoScroll"

#include "ControlPanel.hpp"

#include <SFML/Graphics/Rect.hpp>


inline sf::FloatRect get_universe_rect()
{
    static const sf::FloatRect UNIVERSE(CONTROL_PANEL_HEIGHT, 0,
        WIN_WIDTH, WIN_HEIGHT);
    return UNIVERSE;
}


inline bool outside_universe(const sf::FloatRect& rect)
{
    return !get_universe_rect().Intersects(rect);
}

#endif /* guard WINDOW_HPP */

