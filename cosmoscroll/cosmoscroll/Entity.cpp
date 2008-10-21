#include "Entity.hpp"


Entity::Entity(const sf::Image& img, const sf::Vector2f& offset)
{
    sprite_.SetImage(img);
    sprite_.SetPosition(offset);
    health_ = 3;
}


Entity::~Entity()
{
}


void Entity::Show(sf::RenderWindow& app)
{
    app.Draw(sprite_);
}


void Entity::Hit(int damage)
{
    health_ -= damage;
}


bool Entity::IsDead()
{
    return health_ <= 0;
}


sf::Vector2f Entity::GetPosition()
{
	return sprite_.GetPosition();
}


sf::FloatRect Entity::GetRect()
{
    // Utiliser à la place GetLocalRect avec SFML 1.4
    sf::FloatRect rect;
    rect.Left = sprite_.GetPosition().x;
    rect.Top = sprite_.GetPosition().y;
    rect.Right = rect.Left + sprite_.GetSize().x;    
    rect.Bottom = rect.Top + sprite_.GetSize().y;
    return rect;
}

