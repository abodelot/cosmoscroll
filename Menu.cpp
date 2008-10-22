#include "Menu.hpp"
#include "MediaManager.hpp"


Menu::Menu()
{
    // valeurs par défaut
    textsize_ = 20;
    linespace_ = 10;
    normal_look_.style = sf::String::Regular;
    normal_look_.color = sf::Color::White;
    highlight_look_.style = sf::String::Bold;
    highlight_look_.color = sf::Color::Cyan;
    
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


void Menu::AddItem(const std::string label, int id)
{
    MenuItem item;
    item.label.SetText(label);
    item.label.SetFont(RessourceManager::GetInstance().GetFont());
    item.id = id;
    
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
        ApplyStyle(item, normal_look_);
    }
    items_.push_back(item);
}


bool Menu::ActionChosen(const sf::Event::KeyEvent& key, int& id)
{
    switch (key.Code)
    {
        case sf::Key::Up:
            ApplyStyle(items_[selected_], normal_look_);
            selected_ = (selected_ - 1) % items_.size();
            ApplyStyle(items_[selected_], highlight_look_);
            break;
        case sf::Key::Down:
            ApplyStyle(items_[selected_], normal_look_);
            selected_ = (selected_ + 1) % items_.size();
            ApplyStyle(items_[selected_], highlight_look_);
            break;
        case sf::Key::Return:
            id = items_[selected_].id;
            return true;
        default:
            break;
    }
    return false;
}
    

void Menu::Show(const sf::RenderWindow& app) const
{
    int length = items_.size();
    for (int i = 0; i < length; ++i)
    {
        app.Draw(items_[i].label);
    }
}


void Menu::ApplyStyle(MenuItem& item, const ItemLook& look)
{
    item.label.SetColor(look.color);
    item.label.SetStyle(look.style);
}

