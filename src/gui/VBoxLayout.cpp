#include "VBoxLayout.hpp"
#include "Widget.hpp"

using namespace gui;


VBoxLayout::VBoxLayout(int x, int y)
{
	x_ = x;
	y_ = y;
}

void VBoxLayout::SetSpacing(int vertical_spacing)
{
	vertical_spacing_ = vertical_spacing;
}


Widget* VBoxLayout::Add(Widget* widget)
{
	widget->SetPosition(x_, y_);
	// compute next widget position
	y_ += widget->GetHeight() + vertical_spacing_;
	return widget;
}
