#ifndef GUI_LABEL_HPP
#define GUI_LABEL_HPP

#include "Widget.hpp"

namespace gui
{

class Label: public Widget
{
public:
	Label(Menu* owner, const sf::Unicode::Text& text, int x=0, int y=0);

	// texte du label
	void SetText(const sf::Unicode::Text& text);
	std::string GetText() const { return text_.GetText(); }

	// taille du texte
	void SetSize(int size) { text_.SetSize(size); }
	int GetSize() const { return text_.GetSize(); }

	// text font
	void SetFont(const sf::Font& font) { text_.SetFont(font); }
	const sf::Font& GetFont() const { return text_.GetFont(); }

	// text color
	void SetColor(const sf::Color& color) { text_.SetColor(color); }
	const sf::Color& GetColor() const { return text_.GetColor(); }

private:
	// inherited
	void Render(sf::RenderTarget& target) const;

	sf::String text_;
};

}

#endif // GUI_LABEL_HPP
