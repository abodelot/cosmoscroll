#ifndef GUI_LAYOUT_HPP
#define GUI_LAYOUT_HPP

#include <SFML/System/Vector2.hpp>

namespace gui {

/**
 * Base class for widget layout handler
 */
class Layout {
public:
    Layout(float x, float y);

    void setOffset(float x,float y);
    const sf::Vector2f& getOffset() const;

    /**
     * Holds the spacing between widgets inside the layout.
     */
    void setSpacing(float x, float y);
    const sf::Vector2f& getSpacing() const;

private:
    sf::Vector2f offset_;
    sf::Vector2f spacing_;
};

}

#endif // GUI_LAYOUT_HPP
