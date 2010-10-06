#ifndef GUI_LABEL_HPP
#define GUI_LABEL_HPP

#include "Widget.hpp"

namespace gui
{

class Label: public Widget
{
public:
	Label(Menu* owner, const sf::Unicode::Text& text, int x, int y);

	void SetText(const sf::Unicode::Text& text);

	std::string GetText() const;

	void SetSize(int size);

private:
	// inherited
	void Render(sf::RenderTarget& target) const;

	sf::String text_;
};

}

#endif // GUI_LABEL_HPP
