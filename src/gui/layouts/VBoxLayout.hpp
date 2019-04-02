#ifndef GUI_VBOXLAYOUT_HPP
#define GUI_VBOXLAYOUT_HPP

#include "Layout.hpp"

namespace gui
{

class Widget;

/**
 * Vertical box layout: lines up widgets vertically
 */
class VBoxLayout: public Layout
{
public:
    VBoxLayout(float x, float y);

    Widget* Add(Widget* widget);

};

}

#endif // GUI_VBOXLAYOUT_HPP
