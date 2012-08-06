#include <cassert>

#include "OptionList.hpp"
#include "Menu.hpp"

#define ARROW_MIN_SCALE   0.5f
#define ARROW_MAX_SCALE   1.1f

#define BOX_PADDING      4

using namespace gui;


OptionList::OptionList(Menu* owner) :
	Widget(owner, true)
{
	current_opt_ = -1;
	max_opt_width_ = 0;
	dir_ = 1;
	scale_ = 1;
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


void OptionList::BuildBoxes()
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


void OptionList::AddOption(const sf::String& option)
{
	AddOption(option, "");
}


void OptionList::AddOption(const sf::String& option, const std::string& value)
{
	xsf::Text str;
	str.setString(option);
	str.setCharacterSize(text_size_);
	str.setColor(GetOwner()->GetWidgetStyle().label_text_color);
	str.setFont(*GetOwner()->GetWidgetStyle().global_font);

	if (current_opt_ == -1)
	{
		current_opt_ = 0;
	}
	// resize widget if needed
	int width = str.getWidth();
	if (width > (int) max_opt_width_)
	{
		max_opt_width_ = width;
		BuildBoxes();
	}
	str.setX(ComputeIndentAlign(str));
	std::pair<xsf::Text, std::string> pair(str, value);
	options_.push_back(pair);
}


int OptionList::GetNbItems() const
{
	return options_.size();
}


std::string OptionList::GetOptionAt(int index) const
{
	assert(index >= 0 && index < (int) options_.size());
	const std::pair<sf::Text, std::string>& select = options_[index];
	if (select.second.size() == 0)
	{
		return select.first.getString();
	}
	return select.second;
}


std::string OptionList::GetSelectedOption() const
{
	return GetOptionAt(current_opt_);
}


int OptionList::GetSelectedOptionIndex() const
{
	return current_opt_;
}


void OptionList::Select(int index)
{
	if (index >= 0 && index < (int) options_.size() && index != current_opt_)
	{
		current_opt_ = index;
		CallTheCallback();
	}
}


void OptionList::SelectByValue(const std::string& value)
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


void OptionList::Clear()
{
	current_opt_ = -1;
	options_.clear();
}


void OptionList::SetAlign(Align::EAlign align)
{
	align_ = align;
	for (std::vector<std::pair<xsf::Text, std::string> >::iterator it = options_.begin();
		it != options_.end(); ++it)
	{
		it->first.setX(ComputeIndentAlign(it->first));
	}
}


void OptionList::OnKeyPressed(sf::Keyboard::Key key)
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


void OptionList::OnMouseClicked(int x, int y)
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


void OptionList::OnMouseWheelMoved(int delta)
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


void OptionList::Update(float frametime)
{
	if (scale_ > ARROW_MAX_SCALE || scale_ < ARROW_MIN_SCALE)
	{
		dir_ *= -1;
	}
	scale_ = scale_ + frametime * 0.95 * dir_;
	left_arrow_.setScale(scale_, scale_);
	right_arrow_.setScale(scale_, scale_);
}


int OptionList::ComputeIndentAlign(const xsf::Text& option) const
{
	int base = text_size_ + BOX_PADDING * 3;
	switch (align_)
	{
		case Align::LEFT:
			return base;
		case Align::CENTER:
			return base + (max_opt_width_ - option.getWidth()) / 2;
		case Align::RIGHT:
			return base + max_opt_width_ - option.getWidth();
	}
	return 0;
}


void OptionList::OnStateChanged(State::EState state)
{
	const WidgetStyle& style = GetOwner()->GetWidgetStyle();
	switch (state)
	{
		case State::DEFAULT:
			scale_ = 1;
			left_arrow_.setScale(scale_, scale_);
			right_arrow_.setScale(scale_, scale_);
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


void OptionList::draw(sf::RenderTarget& target, sf::RenderStates states) const
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


int OptionList::PreviousIndex() const
{
	return current_opt_ > 0 ? current_opt_ - 1 : options_.size() - 1;
}


int OptionList::NextIndex() const
{
	return current_opt_ == (int) options_.size() - 1 ? 0 : current_opt_ + 1;
}

