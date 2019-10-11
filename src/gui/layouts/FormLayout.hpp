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

    void addRow(const sf::String& label, Widget* widget);

    Label* getLabelAt(int index);

    void recomputeLabelWidth();

private:
    /**
     * Recompute widgets position
     */
    void alignRows();

    typedef std::pair<Label*, Widget*> FormRow;

    std::vector<FormRow> m_rows;
    int m_maxLabelWidth;
};

}

#endif // GUI_FORMLAYOUT_HPP
