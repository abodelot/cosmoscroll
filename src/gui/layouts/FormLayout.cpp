#include "FormLayout.hpp"

using namespace gui;


FormLayout::FormLayout(float x, float y): Layout(x, y)
{
    label_alignment_ = Align::LEFT;
    label_width_ = 0;
}


void FormLayout::AddRow(const sf::String& str_label, Widget* widget)
{
    Label* label = new Label(widget->GetOwner(), str_label);

    // compute y position from last widget
    int y = 0;
    if (rows_.size() > 0)
    {
        int label_y = rows_.back().first->getPosition().y + rows_.back().first->GetHeight();
        int widget_y = rows_.back().second->getPosition().y + rows_.back().second->GetHeight();
        y = std::max(label_y, widget_y) + GetSpacing().y;
    }
    else
    {
        y = GetOffset().y;
    }
    label->setPosition(label->getPosition().x, y);
    widget->setPosition(widget->getPosition().x, y);

    // label_width_ is the larger label inserted in the layout
    if (label->GetWidth() > label_width_)
        label_width_ = label->GetWidth();



    // append in table
    FormRow row(label, widget);
    rows_.push_back(row);

    AlignRows();
}


void FormLayout::SetLabelAlignment(Align::EAlign align)
{
    label_alignment_ = align;
    AlignRows();
}


Align::EAlign FormLayout::GetLabelAlignmenent() const
{
    return label_alignment_;
}


Label* FormLayout::GetLabelAt(int index)
{
    if (index < (int) rows_.size())
    {
        return rows_[index].first;
    }
    return NULL;
}


void FormLayout::AlignRows()
{

    for (size_t i = 0; i < rows_.size(); ++i)
    {
        Label* current_lab = rows_[i].first;
        int x = GetOffset().x;
        switch (label_alignment_)
        {
            case Align::LEFT:
                x += GetSpacing().x;
                break;
            case Align::RIGHT:
                x += GetSpacing().x + label_width_ - current_lab->GetWidth();
                break;
            default:
                break;
        }
        current_lab->setPosition(x, current_lab->getPosition().y);
        rows_[i].second->setPosition(GetOffset().x + label_width_ + (GetSpacing().x * 2), rows_[i].second->getPosition().y);
    }
}
