#include "Label.hpp"
#include "Menu.hpp"

namespace gui
{

Label::Label(Menu* owner, const sf::String& text, int x, int y) :
    Widget(owner, false)
{
    const WidgetStyle& style = owner->GetWidgetStyle();
    text_.setFillColor(style.label_text_color);
    text_.setFont(*style.global_font);
    text_.setCharacterSize(style.global_text_size);
    text_.setPosition(0, 0);

    setString(text);
    setPosition(x, y);
}


void Label::setString(const sf::String& text)
{
    text_.setString(text);
    const sf::FloatRect& rect = text_.getGlobalBounds();
    Resize(rect.width, rect.height);
}


void Label::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(text_, states);
}

}
