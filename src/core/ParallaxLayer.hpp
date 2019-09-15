#ifndef PARALLAX_LAYER_HPP
#define PARALLAX_LAYER_HPP

#include <SFML/Graphics.hpp>

class ParallaxLayer: public sf::Drawable {
public:
    ParallaxLayer();
    ~ParallaxLayer();

    void scroll(float delta);
    void setTexture(const sf::Texture& texture, bool repeatY);
    void setSpeed(float speed);
    void setColor(const sf::Color& color);
    void setBlendMode(sf::BlendMode mode);

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    float m_speed;
    sf::Sprite m_sprite;
    sf::BlendMode m_blendMode;
};

#endif
