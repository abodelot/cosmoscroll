#ifndef GUI_FORMLAYOUT_HPP
#define GUI_FORMLAYOUT_HPP

#include <vector>

#include "Layout.hpp"
#include "../Label.hpp"

namespace gui
{

/**
 * Convenience layout which manages forms of widgets (right) and their associated labels (left).
 */
class FormLayout: public Layout
{
public:
    FormLayout(float x=0.f, float y=0.f);

    void AddRow(const sf::String& label, Widget* widget);

    /**
     * Holds the horizontal alignment of the labels (left or right).
     */
    void SetLabelAlignment(Align::EAlign align);
    Align::EAlign GetLabelAlignmenent() const;

    Label* GetLabelAt(int index);

    /**
     * Force refresh (if widgets contents have been modified)
     */
    void AlignRows();
private:


    typedef std::pair<Label*, Widget*> FormRow;

    std::vector<FormRow> rows_;
    int label_width_;
    Align::EAlign label_alignment_;
};

}

#endif // GUI_FORMLAYOUT_HPP
