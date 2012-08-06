#ifndef GUI_PROGRESSBAR_HPP
#define GUI_PROGRESSBAR_HPP

#include "Widget.hpp"

namespace gui
{

/**
 * Une barre de progression
 */
class ProgressBar: public Widget
{
public:
	/**
	 * @param owner: menu propriétaire de la barre de progression
	 * @param w: width
	 * @param h: height
	 */
	ProgressBar(Menu* owner, int w, int h);

	void SetValue(int value);

	int GetValue() const;

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::RectangleShape background_;
	sf::Sprite bar_;
	int value_;
};

}

#endif // GUI_PROGRESSBAR_HPP
