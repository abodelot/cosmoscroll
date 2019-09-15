#ifndef GUI_OPTIONLIST_HPP
#define GUI_OPTIONLIST_HPP

#include <vector>
#include <string>

#include "Widget.hpp"

namespace gui {

/**
 * Horizontal select box
 * callback: selection changed
 */
template <class T>
class OptionList: public Widget {
public:
    OptionList(Menu* owner);

    /**
     * Add a choice to the list
     */
    void add(const sf::String& label, const T& value);

    /**
     * Get number of choices
     */
    int getSize() const;

    const T& getValueAt(int index) const;

    /**
     * Get option index currently selected (first is 0)
     */
    int getSelectedIndex() const;

    /**
     * Get selected option value
     * shortcut for GetOptionAt(getSelectedOptionIndex())
     */
    const T& getSelectedValue() const;

    /**
     * Set the current displayed item
     */
    void select(int index);
    void selectByValue(const T& value);

    /**
     * Remove all set options
     */
    void clear();

    void onKeyPressed(sf::Keyboard::Key key) override;
    void onMouseClicked(int x, int y) override;
    void onMouseWheelMoved(int delta) override;

protected:
    void onStateChanged(State::EState state) override;

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void buildBoxes();

    int previousIndex() const;
    int nextIndex() const;

    typedef std::pair<sf::Text, T> Item;
    typedef std::vector<Item> ItemVector;

    ItemVector m_options;
    int m_currentOpt;
    int m_maxOptWidth;
    int m_textSize;
    sf::RectangleShape m_insideBox;
    sf::ConvexShape m_leftArrow;
    sf::ConvexShape m_rightArrow;
};

}

#include "OptionList.inl"

#endif // GUI_OPTIONLIST_HPP
