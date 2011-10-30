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


TextBox::TextBox(Menu* owner, int x, int y, int visible_chars, int max_length):
	Widget(owner, true),
	display_text_(*owner->GetBitmapFont())
{
	visible_chars_ = visible_chars;
	max_length_ = max_length;
	left_offset_ = right_offset_ = 0;
	int height = display_text_.GetFont().GetCharHeight() + PADDING * 2;
	int width = display_text_.GetFont().GetCharWidth() * visible_chars + PADDING * 2;

	box_ = sf::Shape::Rectangle(0, 0, width, height,
		sf::Color::White, OUTLINE, owner->GetWidgetStyle().global_border_color);

	display_text_.SetPosition(PADDING, PADDING);

	cursor_ = sf::Shape::Line(PADDING, PADDING, PADDING, height - PADDING,
		CURSOR_WIDTH, sf::Color::White);
	SetCursor(0);

	SetPosition(x, y);
	Resize(width, height);
	OnStateChanged(GetState());
}


void TextBox::SetText(const sf::Unicode::Text& text)
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
		display_text_.Clear();
		for (int i = 0; i < visible_chars_; ++i)
		{
			display_text_.AppendChar(text_[i]);
		}
	}
	else
	{
		display_text_.SetText(text_);
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
	cursor_.SetColor(sf::Color(64, 64, 255, 255 - (int) alpha));
}


void TextBox::OnTextEntered(sf::Uint32 unicode)
{
	if (unicode >= BitmapFont::FIRST_CHAR && unicode <= BitmapFont::LAST_CHAR)
	{
		if (max_length_ == -1 || (int) text_.length() < max_length_)
		{
			// insertion dans la string d'affichage
			display_text_.InsertChar((char) unicode, cursor_pos_);
			// calcul de la position dans la string réelle
			int position = GetRealCursorPosition();
			text_.insert(position, 1, (char) unicode);
			// si le texte est trop long pour la box
			if (display_text_.Length() > visible_chars_)
			{
				display_text_.RemoveChar(0);
				++left_offset_;
			}
			else
			{
				SetCursor(cursor_pos_ + 1);
			}
		}
	}
}


void TextBox::OnKeyPressed(sf::Key::Code key)
{
	switch (key)
	{
		case sf::Key::Back:
			// supprimer le caractère avant le curseur
			if (cursor_pos_ > 0)
			{
				text_.erase(GetRealCursorPosition() - 1, 1);
				display_text_.RemoveChar(cursor_pos_ - 1);
				if (left_offset_ > 0)
				{
					// le dernier caractère à gauche de la box est inséré au début
					display_text_.InsertChar(text_[left_offset_ - 1], 0);
					--left_offset_;
				}
				else if (right_offset_ > 0)
				{
					// le premier caratère à droite de la box est inséré à la fin
					display_text_.AppendChar(text_[text_.length() - right_offset_]);
					--right_offset_;
					SetCursor(cursor_pos_ - 1);
				}
				else
				{
					SetCursor(cursor_pos_ - 1);
				}
			}
			break;
		case sf::Key::Delete:
			// supprimer le caractère après le curseur
			if (cursor_pos_ < display_text_.Length())
			{
				text_.erase(GetRealCursorPosition(), 1);
				display_text_.RemoveChar(cursor_pos_);
				if (right_offset_ > 0)
				{
					// le premier caratère à droite de la box est inséré à la fin
					display_text_.AppendChar(text_[text_.length() - right_offset_]);
					--right_offset_;
				}
				else if (left_offset_ > 0)
				{
					// le dernier caractère à gauche de la box est inséré au début
					display_text_.InsertChar(text_[left_offset_ - 1], 0);
					--left_offset_;
				}
			}
			break;
		case sf::Key::Left:
			SetCursor(cursor_pos_ - 1);
			break;
		case sf::Key::Right:
			SetCursor(cursor_pos_ + 1);
			break;
		case sf::Key::Home:
			// curseur en début
			SetCursor(0);
			while (left_offset_ > 0)
			{
				ShiftLeft();
			}
			break;
		case sf::Key::End:
			// curseur en fin
			while (right_offset_ > 0)
			{
				ShiftRight();
			}
			SetCursor(display_text_.Length());
			break;
		case sf::Key::Return:
			CallTheCallback();
			break;
		default:
			return;
	}
}


void TextBox::OnMouseClicked(int x, int y)
{
	(void) y;
	// todo: semble marcher, vérifier sous valgrind
	int letter_width = display_text_.GetFont().GetCharWidth();
	int pos = x / letter_width;
	if (pos > display_text_.Length())
	{
		pos = display_text_.Length();
	}
	cursor_.SetX(pos * letter_width);
	cursor_pos_ = pos;
}


void TextBox::OnStateChanged(State::EState state)
{
	const WidgetStyle& style = GetOwner()->GetWidgetStyle();
	switch (state)
	{
		case State::DEFAULT:
			cursor_.SetColor(sf::Color(255, 255, 255, 0));
			box_.SetColor(style.textbox_bg_color);
			break;
		case State::FOCUSED:
			cursor_.SetColor(CURSOR_COLOR);
			box_.SetColor(style.textbox_bg_color_focus);
			break;
		default:
			break;
	}
}


void TextBox::Render(sf::RenderTarget& app) const
{
	app.Draw(box_);
	app.Draw(display_text_);
	app.Draw(cursor_);
}


void TextBox::SetCursor(int position)
{
	if (position == 1 && left_offset_ > 0)
	{
		// il y a des caractères cachés à gauche de la box, on empêche le
		// curseur d'aller plus à gauche
		ShiftLeft();
	}
	else if (position == visible_chars_ && right_offset_ > 0)
	{
		// il a des caractères cachés à droite de la box, on empêche le curseur
		// d'aller plus à droite
		ShiftRight();
	}
	else if (position >= 0 && position < display_text_.Length() + 1)
	{
		// le curseur n'est pas à une extrêmité de la box
		cursor_timer_ = 0.f;
		cursor_pos_ = position;
		cursor_.SetX(position * display_text_.GetFont().GetCharWidth());
	}
}


int TextBox::GetRealCursorPosition() const
{
	return left_offset_ > 0 ? (left_offset_ + cursor_pos_) : cursor_pos_;
}


void TextBox::ShiftLeft()
{
	// ab[cdef]gh => a[bcde]fgh
	display_text_.RemoveChar(-1);
	// le dernier caractère à gauche est réinséré
	display_text_.InsertChar(text_[left_offset_ - 1], 0);
	++right_offset_;
	--left_offset_;
}


void TextBox::ShiftRight()
{
	// ab[cdef]gh => abc[defg]h
	display_text_.RemoveChar(0);
	// le premier caractère à droite est réinséré
	display_text_.AppendChar(text_[text_.length() - right_offset_]);
	--right_offset_;
	++left_offset_;
}

/*
#include <iostream>
void TextBox::Debug() const
{
	std::cout << "real text is:      '" << text_ << "' (" <<  text_.length() << ")" << std::endl;
	std::cout << "displayed text is: '" << display_text_.GetText() << "' (" << display_text_.Length() << ")" << std::endl;
	std::cout << "cursor is at pos:  " << cursor_pos_ << std::endl;
	std::cout << "left offset is:    " << left_offset_ << ", right offset is " << right_offset_ << std::endl;
	std::cout << "----------------------------------------------" << std::endl;
}*/
