#include "Menu.hpp"

#define JOY_DEADZONE	50.f

using namespace gui;


Menu::Menu()
{
	focus_ = NULL;
	focus_index_ = -1;
	bitfont_ = NULL;
	hovered_widget_ = NULL;

	// default theme
	theme_.global_text_size = 20;
	theme_.global_font = &sf::Font::GetDefaultFont();
	theme_.global_border_color = sf::Color(0x80, 0x80, 0x80);

	theme_.label_text_color = sf::Color::White;

	theme_.button_text_color = sf::Color::White;
	theme_.button_text_color_focus = sf::Color::Green;

	theme_.ckbox_v_color = sf::Color(0, 128, 0);
	theme_.ckbox_v_color_focus = sf::Color(0, 192, 0);
	theme_.ckbox_bg_color = sf::Color::White;
	theme_.ckbox_bg_color_focus = sf::Color(239, 228, 176);
	theme_.ckbox_size = 20; // todo

	theme_.slider_handle_color = sf::Color(0x37, 0x73, 0xB3);
	theme_.slider_handle_color_focus = sf::Color(0, 128, 255);
	theme_.slider_bg_color = sf::Color::White;
	theme_.slider_bg_color_focus = sf::Color(239, 228, 176);

	theme_.optlist_arrow_color = sf::Color(0x37, 0x73, 0xB3);
	theme_.optlist_arrow_color_focus = sf::Color(0, 128, 255);
	theme_.optlist_bg_color = sf::Color::White;
	theme_.optlist_bg_color_focus = sf::Color(239, 228, 176);

	theme_.textbox_bg_color = sf::Color::White;
	theme_.textbox_bg_color_focus = sf::Color(239, 228, 176);
}


Menu::~Menu()
{
	for (WidgetList::iterator it = widgets_.begin(); it != widgets_.end(); ++it)
	{
		delete *it;
	}
}


void Menu::OnEvent(const sf::Event& event)
{
	Widget* new_hovered = NULL;
	Widget* new_focus = NULL;
	switch (event.Type)
	{
		case sf::Event::KeyPressed:
			switch (event.Key.Code)
			{
				case sf::Key::Up:
					FocusPreviousWidget();
					break;
				case sf::Key::Down:
					FocusNextWidget();
					break;
				default:
					if (focus_ != NULL)
					{
						focus_->OnKeyPressed(event.Key.Code);
					}
					break;
			}
			break;


		case sf::Event::JoyButtonPressed:
			if (event.JoyButton.Button == 0 && focus_ != NULL)
			{	// map to "return" key
				focus_->OnKeyPressed(sf::Key::Return);
			}
			break;


		case sf::Event::JoyMoved:
			if (event.JoyMove.Axis == sf::Joy::AxisX)
			{
				if (focus_ != NULL)
				{	// map to "left" and "right" keys
					if (event.JoyMove.Position > JOY_DEADZONE)
					{
						focus_->OnKeyPressed(sf::Key::Right);
					}
					else if (event.JoyMove.Position < -JOY_DEADZONE)
					{
						focus_->OnKeyPressed(sf::Key::Left);
					}
				}
			}
			else if (event.JoyMove.Axis == sf::Joy::AxisY)
			{	// map to "up" and "down" key
				if (event.JoyMove.Position > JOY_DEADZONE)
				{
					FocusNextWidget();
				}
				else if (event.JoyMove.Position < -JOY_DEADZONE)
				{
					FocusPreviousWidget();
				}
			}
			break;


		case sf::Event::TextEntered:
			if (focus_ != NULL)
			{
				focus_->OnTextEntered(event.Text.Unicode);
			}
			break;


		case sf::Event::MouseMoved:
			new_hovered = GetHoveredWidget(event.MouseMove.X, event.MouseMove.Y);
			// si un nouveau widget est hovered
			if (new_hovered != NULL && new_hovered != hovered_widget_)
			{
				// si la souris passe instantanément au dessus d'un autre widget,
				// l'ancien widget est toujours en hovered !
				if (hovered_widget_ != NULL)
				{
					hovered_widget_->SetState(focus_ == hovered_widget_ ? State::FOCUSED : State::DEFAULT);
				}
				hovered_widget_ = new_hovered;
				hovered_widget_->SetState(State::HOVERED);
			}
			else if (new_hovered == NULL && hovered_widget_ != NULL)
			{
				// la souris a quitté hovered_widget_
				hovered_widget_->SetState(focus_ == hovered_widget_ ? State::FOCUSED : State::DEFAULT);
				hovered_widget_ = NULL;
			}
			break;


		case sf::Event::MouseButtonPressed:
			// OPTMIZE: utiliser le pointeur hovered_widget_ ?
			// on vérifie si un widget doit prendre le focus (clic souris)
			new_focus = GetHoveredWidget(event.MouseButton.X, event.MouseButton.Y);
			if (new_focus != NULL && new_focus != focus_)
			{
				if (focus_ != NULL)
				{
					focus_->SetState(State::DEFAULT);
				}
				focus_ = new_focus;
				focus_->SetState(State::FOCUSED);
			}
			else if (new_focus == NULL && focus_ != NULL)
			{
				// focus_ a perdu le focus
				focus_->SetState(State::DEFAULT);
				focus_ = NULL;
			}
			break;


		case sf::Event::MouseButtonReleased:
			if (event.MouseButton.Button == sf::Mouse::Left)
			{
				// la souris doit toujours être au-dessus du widget pour
				// que le clic soit prise en compte
				if (hovered_widget_ != NULL && focus_ == hovered_widget_)
				{
				    int x = event.MouseButton.X - focus_->GetPosition().x;
				    int y = event.MouseButton.Y - focus_->GetPosition().y;
					focus_->OnMouseClicked(x, y);
				}
			}
			break;

		case sf::Event::MouseWheelMoved:
			if (focus_ != NULL)
			{
				focus_->OnMouseWheelMoved(event.MouseWheel.Delta);
			}
			break;

		default:
			break;
	}
}


void Menu::Update(float frametime)
{
	if (focus_ != NULL)
	{
		focus_->Update(frametime);
	}
}


void Menu::Show(sf::RenderTarget& target) const
{
	target.Draw(background_);
	for (WidgetList::const_iterator it = widgets_.begin(); it != widgets_.end(); ++it)
	{
		if ((**it).IsVisible())
		{
			target.Draw(**it);
		}
	}
}


void Menu::AddWidget(Widget* widget)
{
	/*WidgetList::iterator it = widgets_.begin();
	while (it != widgets_.end() && (**it).GetPosition().y < widget->GetPosition().y)
	{
		++it;
	}
	widgets_.insert(it, widget);*/
	widgets_.push_back(widget);
	if (focus_index_ == -1)
	{
		FocusWidget(widgets_.size() - 1);
	}
}


void Menu::SetBackground(const sf::Sprite& sprite)
{
	background_ = sprite;
}


void Menu::EventCallback(int) {}


void Menu::LoadBitmapFont(const char* fontpath, int width, int height)
{
	if (bitfont_ != NULL)
	{
		delete bitfont_;
	}
	bitfont_ = new BitmapFont(fontpath, width, height);
}


BitmapFont* Menu::GetBitmapFont() const
{
	return bitfont_;
}


Widget::WidgetStyle& Menu::GetWidgetStyle()
{
	return theme_;
}


const Widget* Menu::GetFocusedWidget() const
{
	return focus_;
}


bool Menu::FocusWidget(int index)
{
	if (index < (int) widgets_.size()
		&& widgets_[index]->CanGrabFocus())
	{
		focus_index_ = index;
		if (focus_ != NULL)
		{
			focus_->SetState(State::DEFAULT);
		}
		focus_ = widgets_[index];
		focus_->SetState(State::FOCUSED);
		return true;
	}
	return false;
}


bool Menu::FocusWidget(const Widget* widget)
{
	for (int i = 0; i < (int) widgets_.size(); ++i)
	{
		if (widgets_[i] == widget)
		{
			return FocusWidget(i);
		}
	}
	return false;
}


bool Menu::FocusFirstWidget()
{
	for (int i = 0; i < (int) widgets_.size(); ++i)
	{
		if (FocusWidget(i))
		{
			return true;
		}
	}
	return false;
}

// TODO: réécrire ce torcheballe (UNE SEULE boucle avec un ternaire next / previous)
// + gérer les widgets disabled
// que faire si tous disabled ?
bool Menu::FocusNextWidget()
{
	for (int i = focus_index_ + 1; i < (int) widgets_.size(); ++i)
	{
		if (FocusWidget(i))
		{
			return true;
		}
	}
	for (int i = 0; i < focus_index_; ++i)
	{
		if (FocusWidget(i))
		{
			return true;
		}
	}
	return false;
}


bool Menu::FocusPreviousWidget()
{
	for (int i = focus_index_ - 1; i >= 0; --i)
	{
		if (FocusWidget(i))
		{
			return true;
		}
	}
	for (int i = (int) widgets_.size() - 1; i > focus_index_; --i)
	{
		if (FocusWidget(i))
		{
			return true;
		}
	}
	return false;
}


Widget* Menu::GetHoveredWidget(int x, int y) const
{
	// transformation des coords absolues en coords relatives
	//x -= GetPosition().x;
	//y -= GetPosition().y;
	WidgetList::const_iterator it = widgets_.begin();
	for (; it != widgets_.end(); ++it)
	{
		if ((**it).CanGrabFocus() && (**it).GetRect().Contains(x, y))
		{
			return *it;
		}
	}
	return NULL;
}


/*void Menu::ActiveFocused()
{
	if (focus_ != NULL)
	{
		focus_->Active();
		OnWidgetActived();
	}
}*/
