#include "Menu.hpp"
#include <cstdio>
#define JOY_DEADZONE	50.f

using namespace gui;


Menu::Menu(WidgetStyle& style)
{
	focus_ = NULL;
	focus_index_ = -1;
	hovered_widget_ = NULL;
	theme_ = &style;
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
	switch (event.type)
	{
		case sf::Event::KeyPressed:
			switch (event.key.code)
			{
				case sf::Keyboard::Up:
					FocusPreviousWidget();
					break;
				case sf::Keyboard::Down:
					FocusNextWidget();
					break;
				default:
					if (focus_ != NULL)
					{
						focus_->OnKeyPressed(event.key.code);
					}
					break;
			}
			break;


		case sf::Event::JoystickButtonPressed:
			if (event.joystickButton.button == 0 && focus_ != NULL)
			{
				// first joystick button is mapped to "return" key
				focus_->OnKeyPressed(sf::Keyboard::Return);
			}
			break;


		case sf::Event::JoystickMoved:
			if (event.joystickMove.axis == sf::Joystick::X)
			{
				if (focus_ != NULL)
				{	// map to "left" and "right" keys
					if (event.joystickMove.position > JOY_DEADZONE)
					{
						focus_->OnKeyPressed(sf::Keyboard::Right);
					}
					else if (event.joystickMove.position < -JOY_DEADZONE)
					{
						focus_->OnKeyPressed(sf::Keyboard::Left);
					}
				}
			}
			else if (event.joystickMove.axis == sf::Joystick::Y)
			{	// map to "up" and "down" key
				if (event.joystickMove.position > JOY_DEADZONE)
				{
					FocusNextWidget();
				}
				else if (event.joystickMove.position < -JOY_DEADZONE)
				{
					FocusPreviousWidget();
				}
			}
			break;


		case sf::Event::TextEntered:
			if (focus_ != NULL)
			{
				focus_->OnTextEntered(event.text.unicode);
			}
			break;


		case sf::Event::MouseMoved:
			new_hovered = GetHoveredWidget(event.mouseMove.x, event.mouseMove.y);
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

				OnWidgetHovered();
			}
			else if (new_hovered == NULL && hovered_widget_ != NULL)
			{
				// la souris a quitté hovered_widget_
				hovered_widget_->SetState(focus_ == hovered_widget_ ? State::FOCUSED : State::DEFAULT);
				hovered_widget_ = NULL;
			}
			break;


		case sf::Event::MouseButtonPressed:
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				// OPTMIZE: utiliser le pointeur hovered_widget_ ?
				// on vérifie si un widget doit prendre le focus (clic souris)
				new_focus = GetHoveredWidget(event.mouseButton.x, event.mouseButton.y);
				if (new_focus != NULL && new_focus != focus_)
				{
					if (focus_ != NULL)
					{
						focus_->SetState(State::DEFAULT);
					}
					focus_ = new_focus;
					focus_->SetState(State::FOCUSED);

					OnWidgetFocused();
				}
				else if (new_focus == NULL && focus_ != NULL)
				{
					// focus_ a perdu le focus
					focus_->SetState(State::DEFAULT);
					focus_ = NULL;
				}
			}
			break;


		case sf::Event::MouseButtonReleased:
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				// la souris doit toujours être au-dessus du widget pour
				// que le clic soit prise en compte
				if (hovered_widget_ != NULL && focus_ == hovered_widget_)
				{
				    int x = event.mouseButton.x - focus_->getPosition().x;
				    int y = event.mouseButton.y - focus_->getPosition().y;
					focus_->OnMouseClicked(x, y);
				}
			}
			break;

		case sf::Event::MouseWheelMoved:
			if (focus_ != NULL)
			{
				focus_->OnMouseWheelMoved(event.mouseWheel.delta);
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
	target.draw(background_);
	for (WidgetList::const_iterator it = widgets_.begin(); it != widgets_.end(); ++it)
	{
		if ((**it).IsVisible())
		{
			target.draw(**it);
		}
	}
}


void Menu::AddWidget(Widget* widget)
{
	/*WidgetList::iterator it = widgets_.begin();
	while (it != widgets_.end() && (**it).getPosition().y < widget->getPosition().y)
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


WidgetStyle& Menu::GetWidgetStyle()
{
	return *theme_;
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

		OnWidgetFocused();
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

// TODO: UNE SEULE boucle avec un ternaire next / previous?
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
	//x -= getPosition().x;
	//y -= getPosition().y;
	WidgetList::const_iterator it = widgets_.begin();
	for (; it != widgets_.end(); ++it)
	{
		if ((**it).CanGrabFocus() && (**it).ContainsPoint(x, y))
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
