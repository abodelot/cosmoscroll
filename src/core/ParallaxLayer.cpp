#include "ParallaxLayer.hpp"
#include "Constants.hpp"

#include <iostream>


ParallaxLayer::ParallaxLayer():
    m_speed(0),
    m_blendMode(sf::BlendAlpha)
{
}


ParallaxLayer::~ParallaxLayer()
{
}


void ParallaxLayer::scroll(float frametime)
{
    const float delta = frametime * m_speed;
    if (m_sprite.getTexture()) {
        m_sprite.move(-delta, 0);
        const sf::Vector2u& texSize = m_sprite.getTexture()->getSize();
        if (-m_sprite.getPosition().x > texSize.x) {
            // Reset x position
            m_sprite.setPosition(0, m_sprite.getPosition().y);
        }
    }
}


void ParallaxLayer::setTexture(const sf::Texture& texture, bool repeatY)
{
    const sf::Vector2u& texSize = texture.getSize();
    sf::Vector2f size(APP_WIDTH, repeatY ? APP_HEIGHT : texSize.y);
    std::cout << "ParallaxLayer setTexture: " << size.x << "x" << size.y << "(repeat=" << repeatY << ")" << std::endl;
    m_sprite.setTexture(texture);
    // Add one extra texture length to allow scrolling
    m_sprite.setTextureRect(sf::IntRect(0, 0, size.x + texSize.x, size.y));
    m_sprite.setPosition(0, repeatY ? 0 : APP_HEIGHT - texSize.y);
}


void ParallaxLayer::setSpeed(float speed)
{
    m_speed = speed;
}


void ParallaxLayer::setColor(const sf::Color& color)
{
    m_sprite.setColor(color);

    // If a color is provided, layer is additive (useful for "fog" top layer)
    m_blendMode = color == sf::Color::White ? sf::BlendAlpha : sf::BlendAdd;
}


void ParallaxLayer::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.blendMode = m_blendMode;
    target.draw(m_sprite, states);
}
