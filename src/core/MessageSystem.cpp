#include "MessageSystem.hpp"

#define MESSAGE_LIFETIME      5.f


MessageSystem::MessageSystem():
    m_speedX(0),
    m_speedY(-50.f),
    m_font(nullptr)
{
}


void MessageSystem::setFont(const sf::Font& font)
{
    m_font = &font;
}


void MessageSystem::write(const sf::String& str, const sf::Vector2f& pos, const sf::Color& color)
{
    Message message;
    message.text.setFont(*m_font);
    message.text.setCharacterSize(10);
    message.text.setString(str);
    message.text.setPosition(pos);
    message.text.setFillColor(color);
    message.lifetime = 0.f;

    m_messages.emplace_back(message);
}


void MessageSystem::update(float frametime)
{
    for (std::vector<Message>::iterator it = m_messages.begin(); it != m_messages.end();) {
        Message& message = *it;
        message.lifetime += frametime;
        if (message.lifetime >= MESSAGE_LIFETIME) {
            it = m_messages.erase(it);
        } else {
            // Move text
            message.text.move(0 * frametime, m_speedY * frametime);

            // Apply fading
            sf::Color color = message.text.getFillColor();
            color.a = (MESSAGE_LIFETIME - message.lifetime) * 255 / MESSAGE_LIFETIME;
            message.text.setFillColor(color);

            ++it;
        }
    }
}


void MessageSystem::clear()
{
    m_messages.clear();
}


void MessageSystem::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (const Message& message: m_messages) {
        target.draw(message.text, states);
    }
}
