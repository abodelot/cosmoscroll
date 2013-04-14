#ifndef MESSAGESYSTEM_HPP
#define MESSAGESYSTEM_HPP

#include <SFML/Graphics.hpp>

/**
 * Static class for displaying animated messages
 */
class MessageSystem
{
public:
	static void setFont(const sf::Font& font);

	/**
	 * Add the message \a str at position \a pos, with color \a color
	 */
	static void write(const sf::String& str, const sf::Vector2f& pos, const sf::Color& color = sf::Color::White);

	/**
	 *
	 */
	static void update(float frametime);

	static void clear();

	static void show(sf::RenderTarget& target, sf::RenderStates states);

private:
	MessageSystem();
	MessageSystem(const MessageSystem&);

	struct Node
	{
		sf::Text text;
		Node*    next;
		float    lifetime;
	};

	static Node*           s_first;
	static const sf::Font* s_font;
};

#endif // MESSAGESYSTEM_HPP
