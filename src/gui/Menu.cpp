#include "Menu.hpp"
#include "Widget.hpp"

#define JOYSTICK_AXIS_THRESHOLD 99.f

using namespace gui;


Menu::Menu(const sf::RenderWindow& window, WidgetStyle& style):
    window_(window),
    theme_(style)
{
    focus_ = NULL;
    focus_index_ = -1;
    hovered_widget_ = NULL;
}


Menu::~Menu()
{
    for (WidgetList::iterator it = widgets_.begin(); it != widgets_.end(); ++it) {
        delete *it;
    }
}


void Menu::onEvent(const sf::Event& event)
{
    Widget* new_hovered = NULL;
    Widget* new_focus = NULL;
    switch (event.type) {
        case sf::Event::KeyPressed:
            switch (event.key.code) {
                case sf::Keyboard::Up:
                    focusPreviousWidget();
                    break;
                case sf::Keyboard::Down:
                    focusNextWidget();
                    break;
                default:
                    if (focus_ != NULL) {
                        focus_->onKeyPressed(event.key.code);
                    }
                    break;
            }
            break;

        case sf::Event::JoystickButtonPressed:
            if (event.joystickButton.button == 0 && focus_ != NULL) {
                // first joystick button is mapped to "return" key
                focus_->onKeyPressed(sf::Keyboard::Return);
            }
            break;

        case sf::Event::JoystickMoved:
            if (event.joystickMove.axis == sf::Joystick::X) {
                if (focus_ != NULL) {    // map to "left" and "right" keys
                    if (event.joystickMove.position > JOYSTICK_AXIS_THRESHOLD) {
                        focus_->onKeyPressed(sf::Keyboard::Right);
                    } else if (event.joystickMove.position < -JOYSTICK_AXIS_THRESHOLD) {
                        focus_->onKeyPressed(sf::Keyboard::Left);
                    }
                }
            } else if (event.joystickMove.axis == sf::Joystick::Y) {    // map to "up" and "down" key
                if (event.joystickMove.position > JOYSTICK_AXIS_THRESHOLD) {
                    focusNextWidget();
                } else if (event.joystickMove.position < -JOYSTICK_AXIS_THRESHOLD) {
                    focusPreviousWidget();
                }
            }
            break;

        case sf::Event::TextEntered:
            if (focus_ != NULL) {
                focus_->onTextEntered(event.text.unicode);
            }
            break;

        case sf::Event::MouseMoved:
            new_hovered = getHoveredWidget(event.mouseMove.x, event.mouseMove.y);
            // si un nouveau widget est hovered
            if (new_hovered != NULL && new_hovered != hovered_widget_) {
                // si la souris passe instantanément au dessus d'un autre widget,
                // l'ancien widget est toujours en hovered !
                if (hovered_widget_ != NULL) {
                    hovered_widget_->setState(focus_ == hovered_widget_ ? State::FOCUSED : State::DEFAULT);
                }
                hovered_widget_ = new_hovered;
                hovered_widget_->setState(State::HOVERED);

                onWidgetHovered();
            } else if (new_hovered == NULL && hovered_widget_ != NULL) {
                // la souris a quitté hovered_widget_
                hovered_widget_->setState(focus_ == hovered_widget_ ? State::FOCUSED : State::DEFAULT);
                hovered_widget_ = NULL;
            }
            break;

        case sf::Event::MouseButtonPressed:
            if (event.mouseButton.button == sf::Mouse::Left) {
                // OPTMIZE: utiliser le pointeur hovered_widget_ ?
                // on vérifie si un widget doit prendre le focus (clic souris)
                new_focus = getHoveredWidget(event.mouseButton.x, event.mouseButton.y);
                if (new_focus != NULL && new_focus != focus_) {
                    if (focus_ != NULL) {
                        focus_->setState(State::DEFAULT);
                    }
                    focus_ = new_focus;
                    focus_->setState(State::FOCUSED);

                    onWidgetFocused();
                } else if (new_focus == NULL && focus_ != NULL) {
                    // focus_ a perdu le focus
                    focus_->setState(State::DEFAULT);
                    focus_ = NULL;
                }
            }
            break;

        case sf::Event::MouseButtonReleased:
            if (event.mouseButton.button == sf::Mouse::Left) {
                // la souris doit toujours être au-dessus du widget pour
                // que le clic soit prise en compte
                if (hovered_widget_ != NULL && focus_ == hovered_widget_) {
                    sf::Vector2f mouse = window_.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
                    mouse.x -= focus_->getPosition().x;
                    mouse.y -= focus_->getPosition().y;
                    focus_->onMouseClicked(mouse.x, mouse.y);
                }
            }
            break;

        case sf::Event::MouseWheelMoved:
            if (focus_ != NULL) {
                focus_->onMouseWheelMoved(event.mouseWheel.delta);
            }
            break;

        default:
            break;
    }
}


void Menu::update(float frametime)
{
    if (focus_ != NULL) {
        focus_->update(frametime);
    }
}


void Menu::show(sf::RenderTarget& target) const
{
    for (WidgetList::const_iterator it = widgets_.begin(); it != widgets_.end(); ++it) {
        if ((**it).isVisible()) {
            target.draw(**it);
        }
    }
}


void Menu::addWidget(Widget* widget)
{
    /*WidgetList::iterator it = widgets_.begin();
    while (it != widgets_.end() && (**it).getPosition().y < widget->getPosition().y)
    {
        ++it;
    }
    widgets_.insert(it, widget);*/
    widgets_.push_back(widget);
    if (focus_index_ == -1) {
        focusWidget(widgets_.size() - 1);
    }
}


WidgetStyle& Menu::getWidgetStyle()
{
    return theme_;
}


const Widget* Menu::getFocusedWidget() const
{
    return focus_;
}


bool Menu::focusWidget(int index)
{
    if (index < (int) widgets_.size()
        && widgets_[index]->canGrabFocus()) {

        focus_index_ = index;
        if (focus_ != NULL) {
            focus_->setState(State::DEFAULT);
        }
        focus_ = widgets_[index];
        focus_->setState(State::FOCUSED);

        onWidgetFocused();
        return true;
    }
    return false;
}


bool Menu::focusWidget(const Widget* widget)
{
    for (int i = 0; i < (int) widgets_.size(); ++i) {
        if (widgets_[i] == widget) {
            return focusWidget(i);
        }
    }
    return false;
}


bool Menu::focusFirstWidget()
{
    for (int i = 0; i < (int) widgets_.size(); ++i) {
        if (focusWidget(i)) {
            return true;
        }
    }
    return false;
}

// TODO: UNE SEULE boucle avec un ternaire next / previous?
// + gérer les widgets disabled
// que faire si tous disabled ?
bool Menu::focusNextWidget()
{
    for (int i = focus_index_ + 1; i < (int) widgets_.size(); ++i) {
        if (focusWidget(i)) {
            return true;
        }
    }
    for (int i = 0; i < focus_index_; ++i) {
        if (focusWidget(i)) {
            return true;
        }
    }
    return false;
}


bool Menu::focusPreviousWidget()
{
    for (int i = focus_index_ - 1; i >= 0; --i) {
        if (focusWidget(i)) {
            return true;
        }
    }
    for (int i = (int) widgets_.size() - 1; i > focus_index_; --i) {
        if (focusWidget(i)) {
            return true;
        }
    }
    return false;
}


Widget* Menu::getHoveredWidget(int x, int y) const
{
    sf::Vector2f mouse = window_.mapPixelToCoords(sf::Vector2i(x, y));
    WidgetList::const_iterator it = widgets_.begin();
    for (; it != widgets_.end(); ++it) {
        if ((**it).canGrabFocus() && (**it).containsPoint(mouse.x, mouse.y)) {
            return *it;
        }
    }
    return NULL;
}

