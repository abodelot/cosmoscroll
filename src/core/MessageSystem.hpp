#ifndef MESSAGESYSTEM_HPP
#define MESSAGESYSTEM_HPP

#include <SFML/Graphics.hpp>
#include <vector>

/**
 * Display animated messages
 */
class MessageSystem: public sf::Drawable {
public:
    MessageSystem();

    void setFont(const sf::Font& font);

    /**
     * Add the message \a str at position \a pos, with color \a color
     */
    void write(const sf::String& str, const sf::Vector2f& pos, const sf::Color& color = sf::Color::White);

    /**
     *
     */
    void update(float frametime);

    void clear();

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    struct Message {
        sf::Text text;
        float lifetime;
    };

    std::vector<Message> m_messages;
    float m_speedX;
    float m_speedY;
    const sf::Font* m_font;
};

#endif
