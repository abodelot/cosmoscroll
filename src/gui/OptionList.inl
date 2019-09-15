#include "Menu.hpp"
#include "utils/SFML_Helper.hpp"

namespace gui {

constexpr float BOX_PADDING = 4;

template <class T>
OptionList<T>::OptionList(Menu* owner) :
    Widget(owner, true)
{
    m_currentOpt = -1;
    m_maxOptWidth = 0;
    const WidgetStyle& style = owner->getWidgetStyle();
    m_textSize = style.global_text_size;

    // left arrow
    m_leftArrow.setPointCount(3);
    m_leftArrow.setPoint(0, sf::Vector2f(0, m_textSize / 2));
    m_leftArrow.setPoint(1, sf::Vector2f(m_textSize, 0));
    m_leftArrow.setPoint(2, sf::Vector2f(m_textSize, m_textSize));
    m_leftArrow.setOrigin(m_textSize / 2, m_textSize / 2);
    m_leftArrow.setPosition(BOX_PADDING + m_textSize / 2, BOX_PADDING + m_textSize / 2);
    m_leftArrow.setOutlineThickness(1);
    m_leftArrow.setOutlineColor(style.global_border_color);

    // right arrow
    m_rightArrow.setPointCount(3);
    m_rightArrow.setPoint(0, sf::Vector2f(0, 0));
    m_rightArrow.setPoint(1, sf::Vector2f(m_textSize, m_textSize / 2));
    m_rightArrow.setPoint(2, sf::Vector2f(0, m_textSize));
    m_rightArrow.setOrigin(m_textSize / 2, m_textSize / 2);

    m_rightArrow.setPosition(0, BOX_PADDING + m_textSize / 2);
    m_rightArrow.setOutlineThickness(1);
    m_rightArrow.setOutlineColor(style.global_border_color);

    // call Resize later, boxes aren't builded yet
    onStateChanged(getState());
    resize(0, Widget::MIN_HEIGHT);
}

template <class T>
void OptionList<T>::buildBoxes()
{
    int m_insideBoxwidth = m_maxOptWidth + BOX_PADDING * 2;
    int arrow_box_width = m_textSize + BOX_PADDING * 2;

    if (m_insideBoxwidth < arrow_box_width)
        m_insideBoxwidth = arrow_box_width;

    int total_width = m_insideBoxwidth + arrow_box_width * 2;
    const WidgetStyle& style = getOwner()->getWidgetStyle();

    m_insideBox.setSize(sf::Vector2f(m_insideBoxwidth, arrow_box_width));
    m_insideBox.setOutlineThickness(1);
    m_insideBox.setOutlineColor(style.global_border_color);
    m_insideBox.setFillColor(style.optlist_bg_color);
    m_insideBox.setPosition(arrow_box_width, 0);

    m_rightArrow.setPosition(arrow_box_width + m_insideBoxwidth + BOX_PADDING + m_textSize / 2, m_rightArrow.getPosition().y);
    setState(isFocused() ? State::FOCUSED : State::DEFAULT);

    resize(total_width, arrow_box_width);
}

template <class T>
void OptionList<T>::add(const sf::String& option, const T& value)
{
    sf::Text str;
    str.setString(option);
    str.setCharacterSize(m_textSize);
    str.setFillColor(getOwner()->getWidgetStyle().label_text_color);
    str.setFont(*getOwner()->getWidgetStyle().global_font);
    str.setOutlineThickness(1.f);
    str.setOutlineColor(sf::Color::Black);

    if (m_currentOpt == -1) {
        m_currentOpt = 0;
    }
    // Resize widget if needed
    int width = sfh::width(str);
    if (width > m_maxOptWidth) {
        m_maxOptWidth = width;
        buildBoxes();
    }
    centerText(str);
    m_options.push_back(Item(str, value));
}

template <class T>
int OptionList<T>::getSize() const
{
    return m_options.size();
}

template <class T>
const T& OptionList<T>::getValueAt(int index) const
{
    const Item& item = m_options.at(index);
    return item.second;
}

template <class T>
const T& OptionList<T>::getSelectedValue() const
{
    return getValueAt(m_currentOpt);
}

template <class T>
int OptionList<T>::getSelectedIndex() const
{
    return m_currentOpt;
}

template <class T>
void OptionList<T>::select(int index)
{
    if (index > 0 && index < (int) m_options.size() && index != m_currentOpt) {
        m_currentOpt = index;
        triggerCallback();
    }
}

template <class T>
void OptionList<T>::selectByValue(const T& value)
{
    for (size_t i = 0; i < m_options.size(); ++i) {
        if (m_options[i].second == value) {
            select(i);
            return;
        }
    }
}

template <class T>
void OptionList<T>::clear()
{
    m_currentOpt = -1;
    m_options.clear();
}

template <class T>
void OptionList<T>::onKeyPressed(sf::Keyboard::Key key)
{
    if (m_options.empty()) {
        return;
    }
    int opt = m_currentOpt;
    switch (key) {
        case sf::Keyboard::Left:
            opt = previousIndex();
            break;
        case sf::Keyboard::Right:
            opt = nextIndex();
            break;
        case sf::Keyboard::Home:
            opt = 0;
            break;
        case sf::Keyboard::End:
            opt = m_options.size() - 1;
            break;
        default:
            break;
    }
    if (opt != m_currentOpt) {
        m_currentOpt = opt;
        triggerCallback();
    }
}

template <class T>
void OptionList<T>::onMouseClicked(int x, int y)
{
    (void) y;
    const int triggerWidth = m_textSize + BOX_PADDING * 2;
    int opt = m_currentOpt;
    if (x > 0 && x < triggerWidth) {
        opt = previousIndex();
    } else if (x > (getWidth() - triggerWidth) && x < getWidth()) {
        opt = nextIndex();
    }

    if (opt != m_currentOpt) {
        m_currentOpt = opt;
        triggerCallback();
    }
}

template <class T>
void OptionList<T>::onMouseWheelMoved(int delta)
{
    if (delta < 0) {
        m_currentOpt = previousIndex();
    } else {
        m_currentOpt = nextIndex();
    }
    triggerCallback();
}

template <class T>
void OptionList<T>::onStateChanged(State::EState state)
{
    const WidgetStyle& style = getOwner()->getWidgetStyle();
    switch (state) {
        case State::DEFAULT:
            m_insideBox.setFillColor(style.optlist_bg_color);
            m_leftArrow.setFillColor(style.optlist_arrow_color);
            m_rightArrow.setFillColor(style.optlist_arrow_color);
            break;
        case State::FOCUSED:
            m_insideBox.setFillColor(style.optlist_bg_color_focus);
            m_leftArrow.setFillColor(style.optlist_arrow_color_focus);
            m_rightArrow.setFillColor(style.optlist_arrow_color_focus);
            break;
        case State::HOVERED:
            m_insideBox.setFillColor(style.optlist_bg_color_focus);
        default:
            break;
    }
}

template <class T>
void OptionList<T>::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(m_insideBox, states);
    target.draw(m_leftArrow, states);
    target.draw(m_rightArrow, states);
    if (m_currentOpt != -1) {
        target.draw(m_options[m_currentOpt].first, states);
    }
}

template <class T>
int OptionList<T>::previousIndex() const
{
    return m_currentOpt > 0 ? m_currentOpt - 1 : m_options.size() - 1;
}

template <class T>
int OptionList<T>::nextIndex() const
{
    return m_currentOpt == (int) m_options.size() - 1 ? 0 : m_currentOpt + 1;
}

}
