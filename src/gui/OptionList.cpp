#include <cassert>

#include "OptionList.hpp"
#include "Menu.hpp"

#define ARROW_MIN_SCALE   0.5f
#define ARROW_MAX_SCALE   1.1f

#define BOX_PADDING      4

using namespace gui;


OptionList::OptionList(Menu* owner, int x, int y) :
	Widget(owner, true)
{
	current_opt_ = -1;
	max_opt_width_ = 0;
	dir_ = 1;
	scale_ = 1;
	align_ = Align::CENTER;
	text_size_ = owner->GetWidgetStyle().global_text_size;

	// left arrow
	left_arrow_.AddPoint(0, text_size_ / 2);
	left_arrow_.AddPoint(text_size_, 0);
	left_arrow_.AddPoint(text_size_, text_size_);
	left_arrow_.SetCenter(text_size_ / 2, text_size_ / 2);
	left_arrow_.SetPosition(BOX_PADDING + text_size_ / 2, BOX_PADDING + text_size_ / 2);
	left_arrow_.SetOutlineWidth(1);
	left_arrow_.SetPointOutlineColor(0, owner->GetWidgetStyle().global_border_color);
	left_arrow_.SetPointOutlineColor(1, owner->GetWidgetStyle().global_border_color);
	left_arrow_.SetPointOutlineColor(2, owner->GetWidgetStyle().global_border_color);

	// right arrow
	right_arrow_.AddPoint(0, 0);
	right_arrow_.AddPoint(text_size_, text_size_ / 2);
	right_arrow_.AddPoint(0, text_size_);
	right_arrow_.SetCenter(text_size_ / 2, text_size_ / 2);
	right_arrow_.SetY(BOX_PADDING + text_size_ / 2);
	right_arrow_.SetOutlineWidth(1);
	right_arrow_.SetPointOutlineColor(0, owner->GetWidgetStyle().global_border_color);
	right_arrow_.SetPointOutlineColor(1, owner->GetWidgetStyle().global_border_color);
	right_arrow_.SetPointOutlineColor(2, owner->GetWidgetStyle().global_border_color);

	// SetRect sera exécuté quand on ajoutera une option, la hauteur et la largeur
	// du widget seront alors connus. On mémorise juste la position en attendant.
	SetPosition(x, y);
	OnStateChanged(GetState());
}


void OptionList::BuildBoxes()
{
	int inside_box_width = max_opt_width_ + BOX_PADDING * 2;
	int box_width = inside_box_width + BOX_PADDING * 4 + text_size_ * 2;
	int pad_text_width = text_size_ + BOX_PADDING * 2;
	WidgetStyle& style = GetOwner()->GetWidgetStyle();

	inside_box_ = sf::Shape::Rectangle(0, 0,
		inside_box_width, pad_text_width, sf::Color::White, 1, style.global_border_color);
	inside_box_.SetX(pad_text_width);

	box_ = sf::Shape::Rectangle(0, 0, box_width, pad_text_width, sf::Color::White, 1, style.global_border_color);
	box_.SetColor(style.optlist_bg_color);

	right_arrow_.SetX(pad_text_width + inside_box_width + BOX_PADDING + text_size_ / 2);
	SetState(IsFocused() ? State::FOCUSED : State::DEFAULT);

	sf::Vector2f pos = GetPosition();
	SetRect(pos.x, pos.y, pos.x + box_width , pos.y + pad_text_width);
}


void OptionList::AddOption(const sf::Unicode::Text& option)
{
	AddOption(option, "");
}


void OptionList::AddOption(const sf::Unicode::Text& option, const std::string& value)
{
	sf::String str;
	str.SetText(option);
	str.SetSize(text_size_);
	str.SetColor(sf::Color::Black);
	str.SetFont(*GetOwner()->GetWidgetStyle().global_font);

	if (current_opt_ == -1)
	{
		current_opt_ = 0;
	}
	// resize widget if needed
	int width = str.GetRect().GetWidth();
	if (width > (int) max_opt_width_)
	{
		max_opt_width_ = width;
		BuildBoxes();
	}
	str.SetX(ComputeIndentAlign(str));
	std::pair<sf::String, std::string> pair(str, value);
	options_.push_back(pair);
}


std::string OptionList::GetSelectedOption() const
{
	assert(current_opt_ >= 0 && current_opt_ < (int) options_.size());
	const std::pair<sf::String, std::string>& select = options_[current_opt_];
	if (select.second.size() == 0)
	{
		return select.first.GetText();
	}
	return select.second;
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
	for (std::vector<std::pair<sf::String, std::string> >::iterator it = options_.begin();
		it != options_.end(); ++it)
	{
		it->first.SetX(ComputeIndentAlign(it->first));
	}
}


void OptionList::OnKeyPressed(sf::Key::Code key)
{
	if (options_.empty())
	{
		return;
	}
	int opt = current_opt_;
	switch (key)
	{
		case sf::Key::Left:
			opt = PreviousIndex();
			break;
		case sf::Key::Right:
			opt = NextIndex();
			break;
		case sf::Key::Home:
			opt = 0;
			break;
		case sf::Key::End:
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
	else if (x > (GetRect().GetWidth() - trigger_width) && x < GetRect().GetWidth())
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
	left_arrow_.SetScale(scale_, scale_);
	right_arrow_.SetScale(scale_, scale_);
}


int OptionList::ComputeIndentAlign(const sf::String& option) const
{
	int base = text_size_ + BOX_PADDING * 3;
	switch (align_)
	{
		case Align::LEFT:
			return base;
		case Align::CENTER:
			return base + (max_opt_width_ - option.GetRect().GetWidth()) / 2;
		case Align::RIGHT:
			return base + max_opt_width_ - option.GetRect().GetWidth();
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
			left_arrow_.SetScale(scale_, scale_);
			right_arrow_.SetScale(scale_, scale_);
			left_arrow_.SetColor(style.optlist_arrow_color);
			right_arrow_.SetColor(style.optlist_arrow_color);
			box_.SetColor(style.optlist_bg_color);
			break;
		case State::FOCUSED:
			left_arrow_.SetColor(style.optlist_arrow_color_focus);
			right_arrow_.SetColor(style.optlist_arrow_color_focus);
			box_.SetColor(style.optlist_bg_color_focus);
			break;
		/*case State::HOVERED:
			box_.SetColor(style.optlist_bg_color_focus);
			break;*/
		default:
			break;
	}
}


void OptionList::ApplyStyle(const WidgetStyle& style)
{
	left_arrow_.SetColor(style.optlist_arrow_color);
	right_arrow_.SetColor(style.optlist_arrow_color);
}


void OptionList::Render(sf::RenderTarget& target) const
{
	target.Draw(box_);
	target.Draw(inside_box_);
	target.Draw(left_arrow_);
	target.Draw(right_arrow_);
	if (current_opt_ != -1)
	{
		target.Draw(options_[current_opt_].first);
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

