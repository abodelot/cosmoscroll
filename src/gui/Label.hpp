#ifndef GUI_LABEL_HPP
#define GUI_LABEL_HPP

#include "Widget.hpp"

namespace gui
{

class Label: public Widget
{
public:
    Label(Menu* owner, const sf::String& text, int x=0, int y=0);

    // texte du label
    void setString(const sf::String& text);
    std::string getString() const { return text_.getString(); }

    // taille du texte
    void setCharacterSize(int size) { text_.setCharacterSize(size); }
    int getCharacterSize() const { return text_.getCharacterSize(); }

    // text font
    void setFont(const sf::Font& font) { text_.setFont(font); }
    const sf::Font& getFont() const { return *text_.getFont(); }

    // text color
    void setColor(const sf::Color& color) { text_.setFillColor(color); }
    const sf::Color& getColor() const { return text_.getFillColor(); }

    // text style
    void setStyle(int flag) { text_.setStyle(flag); }
private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    sf::Text text_;
};

}

#endif // GUI_LABEL_HPP
