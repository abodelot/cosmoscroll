#include "Menu.hpp"
#include "utils/SFML_Helper.hpp"

namespace gui
{

const float BOX_PADDING = 4;

template <class T>
OptionList<T>::OptionList(Menu* owner) :
	Widget(owner, true)
{
	current_opt_ = -1;
	max_opt_width_ = 0;
	align_ = Align::LEFT;
	const WidgetStyle& style = owner->GetWidgetStyle();
	text_size_ = style.global_text_size;

	// left arrow
	left_arrow_.setPointCount(3);
	left_arrow_.setPoint(0, sf::Vector2f(0, text_size_ / 2));
	left_arrow_.setPoint(1, sf::Vector2f(text_size_, 0));
	left_arrow_.setPoint(2, sf::Vector2f(text_size_, text_size_));
	left_arrow_.setOrigin(text_size_ / 2, text_size_ / 2);
	left_arrow_.setPosition(BOX_PADDING + text_size_ / 2, BOX_PADDING + text_size_ / 2);
	left_arrow_.setOutlineThickness(1);
	left_arrow_.setOutlineColor(style.global_border_color);

	// right arrow
	right_arrow_.setPointCount(3);
	right_arrow_.setPoint(0, sf::Vector2f(0, 0));
	right_arrow_.setPoint(1, sf::Vector2f(text_size_, text_size_ / 2));
	right_arrow_.setPoint(2, sf::Vector2f(0, text_size_));
	right_arrow_.setOrigin(text_size_ / 2, text_size_ / 2);

	right_arrow_.setPosition(0, BOX_PADDING + text_size_ / 2);
	right_arrow_.setOutlineThickness(1);
	right_arrow_.setOutlineColor(style.global_border_color);

	// call Resize later, boxes aren't builded yet
	OnStateChanged(GetState());
}

template <class T>
void OptionList<T>::BuildBoxes()
{
	int inside_box_width = max_opt_width_ + BOX_PADDING * 2;
	int arrow_box_width = text_size_ + BOX_PADDING * 2;

	if (inside_box_width < arrow_box_width)
		inside_box_width = arrow_box_width;

	int total_width = inside_box_width + arrow_box_width * 2;
	const WidgetStyle& style = GetOwner()->GetWidgetStyle();

	inside_box_.setSize(sf::Vector2f(inside_box_width, arrow_box_width));
	inside_box_.setOutlineThickness(1);
	inside_box_.setOutlineColor(style.global_border_color);
	inside_box_.setFillColor(style.optlist_bg_color);
	inside_box_.setPosition(arrow_box_width, 0);

	box_.setSize(sf::Vector2f(total_width, arrow_box_width));
	box_.setOutlineColor(style.global_border_color);
	box_.setOutlineThickness(1);
	box_.setFillColor(style.optlist_bg_color);


	right_arrow_.setPosition(arrow_box_width + inside_box_width + BOX_PADDING + text_size_ / 2, right_arrow_.getPosition().y);
	SetState(IsFocused() ? State::FOCUSED : State::DEFAULT);

	Resize(total_width, arrow_box_width);
}

template <class T>
void OptionList<T>::Add(const sf::String& option, const T& value)
{
	sf::Text str;
	str.setString(option);
	str.setCharacterSize(text_size_);
	str.setColor(GetOwner()->GetWidgetStyle().label_text_color);
	str.setFont(*GetOwner()->GetWidgetStyle().global_font);

	if (current_opt_ == -1)
	{
		current_opt_ = 0;
	}
	// resize widget if needed
	int width = sfh::width(str);
	if (width > (int) max_opt_width_)
	{
		max_opt_width_ = width;
		BuildBoxes();
	}
	str.setPosition(ComputeIndentAlign(str));
	options_.push_back(Item(str, value));
}

template <class T>
size_t OptionList<T>::GetSize() const
{
	return options_.size();
}

template <class T>
const T& OptionList<T>::GetValueAt(int index) const
{
	const Item& item = options_.at(index);
	return item.second;
}

template <class T>
const T& OptionList<T>::GetSelectedValue() const
{
	return GetValueAt(current_opt_);
}

template <class T>
int OptionList<T>::GetSelectedIndex() const
{
	return current_opt_;
}

template <class T>
void OptionList<T>::Select(int index)
{
	if (index > 0 && index < (int) options_.size() && index != current_opt_)
	{
		current_opt_ = index;
		CallTheCallback();
	}
}

template <class T>
void OptionList<T>::SelectByValue(const T& value)
{
	for (size_t i = 0; i < options_.size(); ++i)
	{
		if (options_[i].second == value)
		{
			Select(i);
			return;
		}
	}
}

template <class T>
void OptionList<T>::Clear()
{
	current_opt_ = -1;
	options_.clear();
}

template <class T>
void OptionList<T>::SetAlign(Align::EAlign align)
{
	align_ = align;
	for (typename ItemVector::iterator it = options_.begin(); it != options_.end(); ++it)
	{
		it->first.setPosition(ComputeIndentAlign(it->first));
	}
}

template <class T>
void OptionList<T>::OnKeyPressed(sf::Keyboard::Key key)
{
	if (options_.empty())
	{
		return;
	}
	int opt = current_opt_;
	switch (key)
	{
		case sf::Keyboard::Left:
			opt = PreviousIndex();
			break;
		case sf::Keyboard::Right:
			opt = NextIndex();
			break;
		case sf::Keyboard::Home:
			opt = 0;
			break;
		case sf::Keyboard::End:
			opt = options_.size() - 1;
			break;
		default:
			break;
	}
	if (opt != current_opt_)
	{
		current_opt_ = opt;
		CallTheCallback();
	}
}

template <class T>
void OptionList<T>::OnMouseClicked(int x, int y)
{
	(void) y;
	int trigger_width = text_size_ + BOX_PADDING * 2;
	int opt = current_opt_;
	if (x > 0 && x < trigger_width)
	{
		opt = PreviousIndex();
	}
	else if (x > (GetWidth() - trigger_width) && x < GetWidth())
	{
		opt = NextIndex();
	}

	if (opt != current_opt_)
	{
		current_opt_ = opt;
		CallTheCallback();
	}
}

template <class T>
void OptionList<T>::OnMouseWheelMoved(int delta)
{
	if (delta < 0)
	{
		current_opt_ = PreviousIndex();
	}
	else
	{
		current_opt_ = NextIndex();
	}
	CallTheCallback();
}

template <class T>
sf::Vector2f OptionList<T>::ComputeIndentAlign(const sf::Text& option) const
{
	int x = text_size_ + BOX_PADDING * 3;
	int y = (GetHeight() - sfh::height(option)) / 2;
	switch (align_)
	{
		case Align::CENTER:
			x += (max_opt_width_ - sfh::width(option)) / 2;
			break;
		case Align::RIGHT:
			x += max_opt_width_ - sfh::width(option);
			break;
		default:
			break;
	}
	return sf::Vector2f(x, y);
}

template <class T>
void OptionList<T>::OnStateChanged(State::EState state)
{
	const WidgetStyle& style = GetOwner()->GetWidgetStyle();
	switch (state)
	{
		case State::DEFAULT:
			left_arrow_.setFillColor(style.optlist_arrow_color);
			right_arrow_.setFillColor(style.optlist_arrow_color);
			box_.setFillColor(style.optlist_bg_color);
			break;
		case State::FOCUSED:
			left_arrow_.setFillColor(style.optlist_arrow_color_focus);
			right_arrow_.setFillColor(style.optlist_arrow_color_focus);
			box_.setFillColor(style.optlist_bg_color_focus);
			break;
		/*case State::HOVERED:
			box_.setColor(style.optlist_bg_color_focus);
			break;*/
		default:
			break;
	}
}

template <class T>
void OptionList<T>::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(box_, states);
	target.draw(inside_box_, states);
	target.draw(left_arrow_, states);
	target.draw(right_arrow_, states);
	if (current_opt_ != -1)
	{
		target.draw(options_[current_opt_].first, states);
	}
}

template <class T>
int OptionList<T>::PreviousIndex() const
{
	return current_opt_ > 0 ? current_opt_ - 1 : options_.size() - 1;
}

template <class T>
int OptionList<T>::NextIndex() const
{
	return current_opt_ == (int) options_.size() - 1 ? 0 : current_opt_ + 1;
}

}
