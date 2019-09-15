#include "FormLayout.hpp"

using namespace gui;


FormLayout::FormLayout(float x, float y):
    Layout(x, y),
    m_maxLabelWidth(0)
{
}


void FormLayout::addRow(const sf::String& strLabel, Widget* widget)
{
    Label* label = new Label(widget->getOwner(), strLabel);
    m_maxLabelWidth = std::max(m_maxLabelWidth, label->getWidth());

    // Append in form
    m_rows.push_back(FormRow(label, widget));
    alignRows();
}


Label* FormLayout::getLabelAt(int index)
{
    return m_rows.at(index).first;
}


void FormLayout::recomputeLabelWidth()
{
    m_maxLabelWidth = 0;
    for (size_t i = 0; i < m_rows.size(); ++i)
    {
        m_maxLabelWidth = std::max(m_maxLabelWidth, m_rows[i].first->getWidth());
    }
    alignRows();
}


void FormLayout::alignRows()
{
    sf::Vector2f offset = getOffset();
    for (size_t i = 0; i < m_rows.size(); ++i)
    {
        Label* label = m_rows[i].first;
        Widget* widget = m_rows[i].second;

        label->setPosition(offset.x, offset.y);

        float widgetY = offset.y;
        if (widget->getHeight() < Widget::MIN_HEIGHT)
        {
            widgetY += (Widget::MIN_HEIGHT - widget->getHeight()) / 2;
        }
        widget->setPosition(offset.x + m_maxLabelWidth + getSpacing().x, widgetY);
        offset.y += std::max(label->getHeight(), widget->getHeight()) + getSpacing().y;
    }
}
