#include <cassert>

#include "TextBox.hpp"
#include "Menu.hpp"

#define PADDING        3 // marge en pixels autour du texte
#define OUTLINE        1 // bordure de la textbox
#define CURSOR_WIDTH   2
#define CURSOR_COLOR   sf::Color(64, 64, 255)
#define BLINK_PERIOD   1.f

// TODO gérer couleur du curseur dans widget style
using namespace gui;


TextBox::TextBox(Menu* owner, int x, int y, size_t visible_chars, int max_length):
    Widget(owner, true)
{
    const WidgetStyle& style = owner->GetWidgetStyle();
    display_text_.setFont(*style.global_fixed_font);
    display_text_.setColor(style.textbox_color);
    display_text_.setCharacterSize(16);

    visible_chars_ = visible_chars;
    max_length_ = max_length;
    left_offset_ = right_offset_ = 0;

    // Fixed font, using glyph for character 0
    const sf::Glyph& glyph = display_text_.getFont()->getGlyph(0, display_text_.getCharacterSize(), false);
    char_size_ = sf::Vector2i(glyph.bounds.width, glyph.bounds.height);

    float box_height = char_size_.y + PADDING * 2;
    float box_width = char_size_.x * visible_chars + PADDING * 2;

    box_.setSize(sf::Vector2f(box_width, box_height));
    box_.setOutlineThickness(OUTLINE);
    box_.setOutlineColor(style.global_border_color);

    display_text_.setPosition(PADDING, PADDING);

    cursor_.setSize(sf::Vector2f(CURSOR_WIDTH, box_height - PADDING * 2));
    SetCursor(0);

    setPosition(x, y);
    Resize(box_width, box_height);
    OnStateChanged(GetState());
}


void TextBox::setString(const sf::String& text)
{
    left_offset_ = 0;
    text_ = text;
    if ((int) text_.length() > max_length_)
    {
        text_.erase(max_length_);
    }

    int diff = text_.length() - visible_chars_;
    if (diff > 0)
    {
        right_offset_ = diff;
        display_text_.setString(text_.substr(0, visible_chars_));
    }
    else
    {
        display_text_.setString(text_);
    }
    SetCursor(0);
}


const std::string& TextBox::GetText() const
{
    return text_;
}


void TextBox::Update(float frametime)
{
    cursor_timer_ += frametime;
    if (cursor_timer_ >= BLINK_PERIOD)
    {
        cursor_timer_ = 0.f;
    }
    float alpha = 255 * cursor_timer_ / BLINK_PERIOD;
    cursor_.setFillColor(sf::Color(64, 64, 255, 255 - (int) alpha));
}


void TextBox::OnTextEntered(sf::Uint32 unicode)
{
    if (isprint(unicode))
    {
        if (max_length_ == -1 || (int) text_.length() < max_length_)
        {
            // insertion du char dans la string d'affichage
            sf::String str = display_text_.getString();
            str.insert(cursor_pos_, unicode);
            display_text_.setString(str);

            // calcul de la position dans la string réelle
            int position = GetRealCursorPosition();
            text_.insert(position, 1, (char) unicode);
            // si le texte est trop long pour la box, suppr 1er char
            if (display_text_.getString().getSize() > visible_chars_)
            {
                str.erase(0);
                display_text_.setString(str);
                ++left_offset_;
            }
            else
            {
                SetCursor(cursor_pos_ + 1);
            }
        }
    }
}


void TextBox::OnKeyPressed(sf::Keyboard::Key key)
{
    switch (key)
    {
        case sf::Keyboard::BackSpace:
            // supprimer le caractère avant le curseur
            if (cursor_pos_ > 0)
            {
                text_.erase(GetRealCursorPosition() - 1, 1);
                sf::String str2 = display_text_.getString();
                str2.erase(cursor_pos_ - 1);
                display_text_.setString(str2);
                if (left_offset_ > 0)
                {
                    // le dernier caractère à gauche de la box est inséré au début
                    sf::String str = display_text_.getString();
                    str.insert(0, text_[left_offset_ - 1]);
                    display_text_.setString(str);
                    --left_offset_;
                }
                else if (right_offset_ > 0)
                {
                    // le premier caratère à droite de la box est inséré à la fin
                    sf::String str = display_text_.getString();
                    str.insert(str.getSize(), text_[text_.length() - right_offset_]);
                    display_text_.setString(str);
                    --right_offset_;
                    SetCursor(cursor_pos_ - 1);
                }
                else
                {
                    SetCursor(cursor_pos_ - 1);
                }
            }
            break;
        case sf::Keyboard::Delete:
            // supprimer le caractère après le curseur
            if (cursor_pos_ < display_text_.getString().getSize())
            {
                text_.erase(GetRealCursorPosition(), 1);
                sf::String str = display_text_.getString();
                str.erase(cursor_pos_);
                if (right_offset_ > 0)
                {
                    // le premier caratère à droite de la box est inséré à la fin
                    str.insert(str.getSize(), text_[text_.length() - right_offset_]);
                    --right_offset_;
                }
                else if (left_offset_ > 0)
                {
                    // le dernier caractère à gauche de la box est inséré au début
                    str.insert(0, text_[left_offset_ - 1]);
                    --left_offset_;
                }
                display_text_.setString(str);
            }
            break;
        case sf::Keyboard::Left:
            SetCursor(cursor_pos_ - 1);
            break;
        case sf::Keyboard::Right:
            SetCursor(cursor_pos_ + 1);
            break;
        case sf::Keyboard::Home:
            // curseur en début
            SetCursor(0);
            while (left_offset_ > 0)
            {
                ShiftLeft();
            }
            break;
        case sf::Keyboard::End:
            // curseur en fin
            while (right_offset_ > 0)
            {
                ShiftRight();
            }
            SetCursor(display_text_.getString().getSize());
            break;
        case sf::Keyboard::Return:
            CallTheCallback();
            break;
        default:
            return;
    }
}


void TextBox::OnMouseClicked(int x, int y)
{
    (void) y;
    int pos = x / char_size_.x;
    if (pos > display_text_.getString().getSize())
    {
        pos = display_text_.getString().getSize();
    }
    SetCursor(pos);
}


void TextBox::OnStateChanged(State::EState state)
{
    const WidgetStyle& style = GetOwner()->GetWidgetStyle();
    switch (state)
    {
        case State::DEFAULT:
            cursor_.setFillColor(sf::Color(255, 255, 255, 0));
            box_.setFillColor(style.textbox_bg_color);
            break;
        case State::FOCUSED:
            cursor_.setFillColor(CURSOR_COLOR);
            box_.setFillColor(style.textbox_bg_color_focus);
            break;
        default:
            break;
    }
}


void TextBox::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(box_, states);
    target.draw(display_text_, states);
    target.draw(cursor_, states);
}


void TextBox::SetCursor(int position)
{
    if (position == 1 && left_offset_ > 0)
    {
        // il y a des caractères cachés à gauche de la box, on empêche le curseur d'aller plus à gauche
        ShiftLeft();
    }
    else if (position == visible_chars_ && right_offset_ > 0)
    {
        // il a des caractères cachés à droite de la box, on empêche le curseur d'aller plus à droite
        ShiftRight();
    }
    else if (position >= 0 && position < display_text_.getString().getSize() + 1)
    {
        // le curseur n'est pas à une extrêmité de la box
        cursor_timer_ = 0.f;
        cursor_pos_ = position;
        cursor_.setPosition(PADDING + position * char_size_.x, PADDING);
    }
}


int TextBox::GetRealCursorPosition() const
{
    return left_offset_ > 0 ? (left_offset_ + cursor_pos_) : cursor_pos_;
}


void TextBox::ShiftLeft()
{
    // ab[cdef]gh => a[bcde]fgh
    sf::String str = display_text_.getString();
    str.erase(str.getSize() - 1);
    // le dernier caractère à gauche est réinséré
    str.insert(0, text_[left_offset_ - 1]);
    display_text_.setString(str);
    ++right_offset_;
    --left_offset_;
}


void TextBox::ShiftRight()
{
    // ab[cdef]gh => abc[defg]h
    sf::String str = display_text_.getString();
    str.erase(0);
    // le premier caractère à droite est réinséré
    str.insert(str.getSize(), text_[text_.length() - right_offset_]);
    display_text_.setString(str);
    --right_offset_;
    ++left_offset_;
}
