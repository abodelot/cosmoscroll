#include "Entity.hpp"
#include "Bonus.hpp"
#include "Window.hpp"

#include <typeinfo>


Entity::Entity(const sf::Image& img, const sf::Vector2f& offset, int hp)
{
    sprite_.SetImage(img);
    sprite_.SetPosition(offset);
    hp_ = hp;
}


Entity::~Entity()
{
}

void Entity::Action()
{
}



void Entity::Show(sf::RenderWindow& app) const
{
    app.Draw(sprite_);
}


void Entity::Hit(int damage)
{
    hp_ -= damage;
}


void Entity::Collide(Entity& ent)
{
    // TODO: gérer les collisions de façon plus réaliste, l'angle de this
    // devrait être modifié en fonction de l'angle de ent
    // -> comment définir GetAngle() pour chaque entité ?
    // -> trouver la formule physique adéquate pour gérer les rebonds
    (void) ent;
    --hp_;
}


bool Entity::IsDead()
{
    return hp_ <= 0;
}


void Entity::Kill()
{
    hp_ = 0;
}


void Entity::KillIfOut()
{
    if (outside_universe(GetRect()))
    {
        hp_ = 0;
        puts("(entity is out)");
    }
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


