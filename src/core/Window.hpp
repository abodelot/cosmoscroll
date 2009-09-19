#ifndef WINDOW_HPP
#define WINDOW_HPP

// constantes de configuration de la fenêtre
#define WIN_WIDTH   640
#define WIN_HEIGHT  480
#define WIN_BPP     32
#define WIN_FPS     60
#define WIN_TITLE   "CosmoScroll"

#ifndef SVN_REV
#define SVN_REV "???"
#endif

#define COSMOSCROLL_VERSION "0.2-devel"
#define COSMOSCROLL_ABOUT str_sprintf(\
		L"À propos de CosmoScroll\n\n" \
		"Version : %s\n\n" \
		"Auteurs :\n" \
		"     Alexandre Bodelot\n" \
		"     Arnaud Wolff\n\n" \
		"Licence : GPL", COSMOSCROLL_VERSION)/*, SVN_REV)*/

#include <SFML/Graphics/Rect.hpp>

#include "ControlPanel.hpp"


inline bool outside_universe(const sf::FloatRect& rect)
{
	static const sf::FloatRect UNIVERSE(0, ControlPanel::HEIGHT, WIN_WIDTH,
		WIN_HEIGHT);
	return !UNIVERSE.Intersects(rect);
}

#endif // WINDOW_HPP

