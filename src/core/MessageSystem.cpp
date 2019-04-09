#include "MessageSystem.hpp"

#define MESSAGE_LIFETIME      5.f
#define MESSAGE_SPEED_X       0.f
#define MESSAGE_SPEED_Y     -50.f


MessageSystem::Node* MessageSystem::s_first = NULL;
const sf::Font*      MessageSystem::s_font  = NULL;


void MessageSystem::setFont(const sf::Font& font)
{
    s_font = &font;
}


void MessageSystem::write(const sf::String& str, const sf::Vector2f& pos, const sf::Color& color)
{
    Node* node = new Node;
    node->text.setFont(*s_font);
    node->text.setCharacterSize(10);
    node->text.setString(str);
    node->text.setPosition(pos);
    node->text.setFillColor(color);
    node->lifetime = 0.f;

    // Prepend the node in the linked list
    node->next = s_first;
    s_first = node;
}


void MessageSystem::update(float frametime)
{
    Node* node = s_first;
    Node* previous = NULL;
    while (node != NULL)
    {
        node->lifetime += frametime;
        if (node->lifetime >= MESSAGE_LIFETIME)
        {
            // Previous node must be linked to the next node
            if (previous == NULL)
                s_first = node->next;
            else
                previous->next = node->next;

            // Delete current node
            Node* next = node->next;
            delete node;
            node = next;
        }
        else
        {
            // Move text
            node->text.move(MESSAGE_SPEED_X * frametime, MESSAGE_SPEED_Y * frametime);

            // Apply fading
            sf::Color color = node->text.getFillColor();
            color.a = (MESSAGE_LIFETIME - node->lifetime) * 255 / MESSAGE_LIFETIME;
            node->text.setFillColor(color);

            previous = node;
            node = node->next;
        }
    }
}


void MessageSystem::clear()
{
    Node* node = s_first;
    while (node != NULL)
    {
        Node* next = node->next;
        delete node;
        node = next;
    }
    s_first = NULL;
}


void MessageSystem::show(sf::RenderTarget& target, sf::RenderStates states)
{
    for (Node* node = s_first; node != NULL; node = node->next)
    {
        target.draw(node->text, states);
    }
}
