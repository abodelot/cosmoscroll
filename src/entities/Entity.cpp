#include "Entity.hpp"
#include "Bonus.hpp"
#include "../core/Window.hpp"


Entity::Entity(const sf::Image& img, const sf::Vector2f& offset, int hp)
{
	id_ = -1;

	SetImage(img);
	SetPosition(offset);
	hp_ = hp;
	flipped_ = false;
}


Entity::~Entity()
{
}


void Entity::Hit(int damage)
{
	hp_ -= damage;
}


void Entity::OnCollide(Entity& entity)
{
	// TODO: gérer les collisions de façon plus réaliste, l'angle de this
	// devrait être modifié en fonction de l'angle de ent
	// -> comment définir GetAngle() pour chaque entité ?
	// -> trouver la formule physique adéquate pour gérer les rebonds
	(void) entity;
	Hit(1);
}


void Entity::KillIfOut()
{
	if (outside_universe(GetCollideRect()))
	{
		hp_ = 0;
#ifdef DEBUG_ENTITY
		sf::Vector2f pos = GetPosition();
		printf("(entity is out : x %f, y %f)\n", pos.x, pos.y);
#endif
	}
}


sf::FloatRect Entity::GetCollideRect() const
{
	sf::FloatRect rect;
	rect.Left = GetPosition().x;
	rect.Top = GetPosition().y;
	rect.Right = rect.Left + GetSize().x;
	rect.Bottom = rect.Top + GetSize().y;
	return rect;
}


void Entity::SetID(int id)
{
	id_ = id;
}


int Entity::GetID() const
{
	return id_;
}
