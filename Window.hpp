#ifndef WINDOW_HPP
#define WINDOW_HPP

// constantes de configuration de la fenêtre
#define WIN_WIDTH   640
#define WIN_HEIGHT  480
#define WIN_BPP     32
#define WIN_FPS     60
#define WIN_TITLE   "CosmoScroll"

#define COSMOSCROLL_VERSION "devel" // 0.1
#define COSMOSCROLL_ABOUT str_sprintf(\
		L"À propos de CosmoScroll\n\n" \
		"Version : %s (svn%s)\n" \
		"Auteurs :\n" \
		"\tAlexandre Bodelot\n" \
		"\tArnaud Wolff\n" \
		"\nLicence : GPL", COSMOSCROLL_VERSION, SVN_REV)

#include "ControlPanel.hpp"

#include <SFML/Graphics/Rect.hpp>


inline bool outside_universe(const sf::FloatRect& rect)
{
	static const sf::FloatRect UNIVERSE(0, CONTROL_PANEL_HEIGHT, WIN_WIDTH,
		WIN_HEIGHT);
	return !UNIVERSE.Intersects(rect);
}

#endif /* guard WINDOW_HPP */

