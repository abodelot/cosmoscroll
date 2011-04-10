#ifndef GUI_LABEL_HPP
#define GUI_LABEL_HPP

#include "Widget.hpp"

namespace gui
{

class Label: public Widget
{
public:
	Label(Menu* owner, const sf::Unicode::Text& text, int x, int y);

	// texte du label
	void SetText(const sf::Unicode::Text& text) { text_.SetText(text); }
	std::string GetText() const { return text_.GetText(); }

	// taille du texte
	void SetSize(int size) { text_.SetSize(size); }
	int GetSize() const { return text_.GetSize(); }

	// fonte du texte
	void SetFont(const sf::Font& font) { text_.SetFont(font); }
	const sf::Font& GetFont() const { return text_.GetFont(); }

private:
	// inherited
	void Render(sf::RenderTarget& target) const;

	sf::String text_;
};

}

#endif // GUI_LABEL_HPP
