#include <cassert>

#include "Menu.hpp"
#include "../core/SoundSystem.hpp"
#include "../utils/MediaManager.hpp"


Menu::Menu()
{
	// valeurs par défaut
	textsize_ = 20;
	linespace_ = 10;
	normal_look_.style = sf::String::Regular;
	normal_look_.color = sf::Color::White;
	highlight_look_.style = sf::String::Underlined;
	highlight_look_.color = sf::Color::Green;
	unactive_look_.style = sf::String::Regular;
	unactive_look_.color = sf::Color(0x50, 0x50, 0x50);
	// pas encore d'éléments, donc pas de sélection
	selected_ = -1;
}


void Menu::SetNormalLook(unsigned long style, const sf::Color& color)
{
	normal_look_.style = style;
	normal_look_.color = color;
}


void Menu::SetHighlightLook(unsigned long style, const sf::Color& color)
{
	highlight_look_.style = style;
	highlight_look_.color = color;
}


void Menu::SetTextSize(int size)
{
	textsize_ = size;
}


void Menu::SetOffset(const sf::Vector2f& offset)
{
	offset_ = offset;
}


void Menu::SetLineSpace(int linespace)
{
	linespace_ = linespace;
}


void Menu::AddItem(const std::wstring label, int id, bool activable)
{
	MenuItem item;
	item.label.SetText(label);
	item.label.SetFont(GET_FONT());
	item.id = id;
	item.activable = activable;

	// si on a ajouté le premier élément
	if (selected_ == -1)
	{
		selected_ = 0;
		item.label.SetPosition(offset_);
		ApplyStyle(item, highlight_look_);
	}
	else
	{
		sf::FloatRect rect = items_.back().label.GetRect();
		item.label.SetPosition(rect.Left, rect.Bottom + linespace_);
		ApplyStyle(item, activable ? normal_look_ : unactive_look_);
	}
	items_.push_back(item);
}


void Menu::AddItem(const std::string label, int id, bool activable)
{
	MenuItem item;
	item.label.SetText(label);
	item.label.SetFont(GET_FONT());
	item.id = id;
	item.activable = activable;

	// si on a ajouté le premier élément
	if (selected_ == -1)
	{
		selected_ = 0;
		item.label.SetPosition(offset_);
		ApplyStyle(item, highlight_look_);
	}
	else
	{
		sf::FloatRect rect = items_.back().label.GetRect();
		item.label.SetPosition(rect.Left, rect.Bottom + linespace_);
		ApplyStyle(item, activable ? normal_look_ : unactive_look_);
	}
	items_.push_back(item);
}


bool Menu::ItemChosen(Input::Action action, int& id)
{
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
				id = items_[selected_].id;
				SoundSystem::GetInstance().PlaySound("menu-valid");
				return true;
			}
			SoundSystem::GetInstance().PlaySound("menu-non-activable");
			break;
		default:
			break;
	}
	return false;
}


void Menu::SelectItem(int n)
{
	assert(n >= 0 && n < (int) items_.size());
	bool activable = items_[selected_].activable;
	ApplyStyle(items_[selected_], activable ? normal_look_ : unactive_look_);
	ApplyStyle(items_[n], highlight_look_);
	selected_ = n;
}


void Menu::Show(sf::RenderWindow& app) const
{
	int length = items_.size();
	for (int i = 0; i < length; ++i)
	{
		app.Draw(items_[i].label);
	}
}


void Menu::Clear()
{
	items_.clear();
	selected_ = -1;
}


void Menu::ResetStyle(MenuItem& item)
{
	ApplyStyle(item, item.activable ? normal_look_ : unactive_look_);
}


void Menu::ApplyStyle(MenuItem& item, const ItemLook& look)
{
	item.label.SetColor(look.color);
	item.label.SetStyle(look.style);
}
