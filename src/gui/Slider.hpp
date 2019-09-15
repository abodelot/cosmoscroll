#ifndef GUI_SLIDER_HPP
#define GUI_SLIDER_HPP

#include "Widget.hpp"

namespace gui {

/**
 * Callback appelée quand : valeur modifiée
 */
class Slider: public Widget {
public:
    /**
     * @param owner: menu propriétaire du slider
     * @param w: largeur
     * @param h: hauteur (-1 => taille de texte du menu)
     */
    Slider(Menu* owner, int w, int h=-1);

    /**
     * Indiquer le pas de variation de la poignée
     */
    void setQuantum(int quantum);

    int getValue() const;
    void setValue(int value);

    void onKeyPressed(sf::Keyboard::Key code) override;
    void onMouseClicked(int x, int y) override;
    void onMouseWheelMoved(int delta) override;

protected:
    void onStateChanged(State::EState state) override;

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void updateHandle(int value);

    sf::RectangleShape bar_;
    sf::RectangleShape handle_;
    int handle_index_;
    int quantum_;
};

}

#endif // GUI_SLIDER_HPP
