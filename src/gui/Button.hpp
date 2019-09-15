#ifndef GUI_BUTTON_HPP
#define GUI_BUTTON_HPP

#include "Widget.hpp"

namespace gui
{

/**
 * Un bouton cliquable
 * callback si : touche <Enter>
 */
class Button: public Widget
{
public:
    /**
     * @param owner: menu propriétaire du bouton
     * @param text: contenu affiché
     * @param w: largeur (-1 pour largeur automatiquement adaptée au texte)
     * @param h: hauteur (-1 pour hauteur automatiquement adaptée au texte)
     */
    Button(Menu* owner, const sf::String& text, int w = -1, int h = -1);

    /**
     * Indiquer le texte affiché sur le bouton
     */
    void setString(const sf::String& text);

    void setTextSize(int size);

    void setTextColor(const sf::Color& color);

    virtual void onKeyPressed(sf::Keyboard::Key code) override;
    virtual void onMouseClicked(int, int) override;

protected:
    virtual void onStateChanged(State::EState state) override;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    sf::Text text_;
};

}

#endif // GUI_BUTTON_HPP
