#ifndef GUI_VBOXLAYOUT_HPP
#define GUI_VBOXLAYOUT_HPP


namespace gui
{

class Widget;

class VBoxLayout
{
public:
	VBoxLayout(int x, int y);

	void SetSpacing(int vertical_spacing);

	Widget* Add(Widget* widget);

private:
	int x_;
	int y_;
	int vertical_spacing_;
};

}

#endif // GUI_VBOXLAYOUT_HPP
