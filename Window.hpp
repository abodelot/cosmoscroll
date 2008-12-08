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

#define ABOUT_TEXT "BLAH BLAH ABOUT TEXT HERE\n -\n\n changer de header"


inline bool outside_universe(const sf::FloatRect& rect)
{
	static const sf::FloatRect UNIVERSE(0, CONTROL_PANEL_HEIGHT, WIN_WIDTH,
		WIN_HEIGHT);
	return !UNIVERSE.Intersects(rect);
}

#endif /* guard WINDOW_HPP */

