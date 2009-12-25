#include <cassert>

#include "BaseMenu.hpp"
#include "../core/Game.hpp"
#include "../core/Input.hpp"
#include "../core/SoundSystem.hpp"
#include "../utils/MediaManager.hpp"


BaseMenu::BaseMenu()
{
	background_.SetImage(GET_IMG("gui/background"));
	background2_.SetImage(GET_IMG("gui/main-screen"));

	// default settings
	textsize_ = 32;
	linespace_ = 10;
	offset_y_ = 10;
	normal_look_.style = sf::String::Regular;
	normal_look_.color = sf::Color::White;
	highlight_look_.style = sf::String::Underlined;
	highlight_look_.color = sf::Color::Green;
	unactive_look_.style = sf::String::Regular;
	unactive_look_.color = sf::Color(0x50, 0x50, 0x50);
	// no elements yet, no selection
	selected_ = -1;
}


void BaseMenu::OnEvent(const sf::Event& event)
{
	Input::Action action = Input::GetInstance().EventToAction(event);
	switch (action)
	{
		case Input::MOVE_UP:
			ResetStyle(items_[selected_]);
			selected_ = selected_ == 0 ? items_.size() - 1 : selected_ - 1;
			ApplyStyle(items_[selected_], highlight_look_);
			SoundSystem::GetInstance().PlaySound("menu-select");
			break;
		case Input::MOVE_DOWN:
			ResetStyle(items_[selected_]);
			selected_ = (selected_ + 1) % items_.size();
			ApplyStyle(items_[selected_], highlight_look_);
			SoundSystem::GetInstance().PlaySound("menu-select");
			break;
		case Input::ENTER:
			if (items_[selected_].activable)
			{
				Callback(items_[selected_].id);
				SoundSystem::GetInstance().PlaySound("menu-valid");
			}
			else
			{
				SoundSystem::GetInstance().PlaySound("menu-non-activable");
			}
			break;
		default:
			break;
	}
}


void BaseMenu::Update(float frametime)
{
	static const int SPEED = 30;
	static float y = 0;
	y -= SPEED * frametime;
	if (y < 0)
	{
		y = Game::HEIGHT;
	}
	background_.SetY(y);
}


void BaseMenu::Show(sf::RenderTarget& target) const
{
	// drawing background twice for scrolling purpose
	target.Draw(background_);
	background_.SetCenter(0.f, background_.GetSize().y);
	target.Draw(background_);
	background_.SetCenter(0.f, 0.f);

	target.Draw(background2_);

	DrawItems(target);
}


void BaseMenu::DrawItems(sf::RenderTarget& target) const
{
	target.Draw(title_);
	const int length = items_.size();
	for (int i = 0; i < length; ++i)
	{
		target.Draw(items_[i].label);
	}
}


void BaseMenu::SetNormalLook(unsigned long style, const sf::Color& color)
{
	normal_look_.style = style;
	normal_look_.color = color;
}


void BaseMenu::SetHighlightLook(unsigned long style, const sf::Color& color)
{
	highlight_look_.style = style;
	highlight_look_.color = color;
}


void BaseMenu::SetTextSize(int size)
{
	textsize_ = size;
}


void BaseMenu::SetOffsetY(float y)
{
	offset_y_ = y;
}


void BaseMenu::SetLineSpace(int linespace)
{
	linespace_ = linespace;
}


void BaseMenu::SetBackground(const sf::Image& image)
{
	background_.SetImage(image);
}


void BaseMenu::AddOption(const sf::Unicode::Text& label, int id, bool activable)
{
	MenuItem item;
	item.label.SetText(label);
	item.label.SetFont(GET_FONT());
	item.label.SetSize(textsize_);
	item.id = id;
	item.activable = activable;

	// if we are about to insert the first element
	if (selected_ == -1)
	{
		selected_ = 0;
		item.label.SetY(offset_y_);
		ApplyStyle(item, highlight_look_);
	}
	else
	{
		sf::FloatRect rect = items_.back().label.GetRect();
		item.label.SetY(rect.Bottom + linespace_);
		ApplyStyle(item, activable ? normal_look_ : unactive_look_);
	}
	item.label.SetX((Game::WIDTH - item.label.GetRect().GetWidth()) / 2);
	items_.push_back(item);
}


void BaseMenu::SetTextOption(const sf::Unicode::Text& text, int id)
{
	std::vector<MenuItem>::iterator it;
	for (it = items_.begin(); it != items_.end(); ++it)
	{
		if (it->id == id)
		{
			it->label.SetText(text);
			break;
		}
	}
}


void BaseMenu::SelectItem(int n)
{
	assert(n >= 0 && n < (int) items_.size());
	bool activable = items_[selected_].activable;
	ApplyStyle(items_[selected_], activable ? normal_look_ : unactive_look_);
	ApplyStyle(items_[n], highlight_look_);
	selected_ = n;
}


void BaseMenu::Clear()
{
	items_.clear();
	selected_ = -1;
}


void BaseMenu::Callback(int)
{
}


void BaseMenu::SetTitle(const wchar_t* text, int y)
{
	title_.SetFont(GET_FONT());
	title_.SetSize(50);
	title_.SetText(text);
	title_.SetX((Game::WIDTH - title_.GetRect().GetWidth()) / 2);
	title_.SetY(y);
}


void BaseMenu::ApplyStyle(MenuItem& item, const ItemLook& look)
{
	item.label.SetColor(look.color);
	item.label.SetStyle(look.style);
}


void BaseMenu::ResetStyle(MenuItem& item)
{
	ApplyStyle(item, item.activable ? normal_look_ : unactive_look_);
}


