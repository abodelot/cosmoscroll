#include "FormLayout.hpp"

using namespace gui;


FormLayout::FormLayout(float x, float y):
    Layout(x, y),
    m_maxLabelWidth(0)
{
}


void FormLayout::addRow(const sf::String& strLabel, Widget* widget)
{
    Label* label = new Label(widget->GetOwner(), strLabel);
    m_maxLabelWidth = std::max(m_maxLabelWidth, label->GetWidth());

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
        m_maxLabelWidth = std::max(m_maxLabelWidth, m_rows[i].first->GetWidth());
    }
    alignRows();
}


void FormLayout::alignRows()
{
    sf::Vector2f offset = GetOffset();
    for (size_t i = 0; i < m_rows.size(); ++i)
    {
        Label* label = m_rows[i].first;
        Widget* widget = m_rows[i].second;

        label->setPosition(offset.x, offset.y);

        float widgetY = offset.y;
        if (widget->GetHeight() < Widget::MIN_HEIGHT)
        {
            widgetY += (Widget::MIN_HEIGHT - widget->GetHeight()) / 2;
        }
        widget->setPosition(offset.x + m_maxLabelWidth + GetSpacing().x, widgetY);
        offset.y += std::max(label->GetHeight(), widget->GetHeight()) + GetSpacing().y;
    }
}
