#include <typeinfo>

#include "Entity.hpp"
#include "Bonus.hpp"
#include "Window.hpp"


Entity::Entity(const sf::Image& img, const sf::Vector2f& offset, int hp)
{
	SetImage(img);
	SetPosition(offset);
	hp_ = hp;
	flipped_ = false;
	
	//	Le but: Limiter les positions des mouvements des entités
	/*use_limits_ = false;
	x_limits_.x = 0;
	x_limits_.y = WIN_WIDTH;
	
	y_limits_.x = CONTROL_PANEL_HEIGHT;
	y_limits_.y = WIN_HEIGHT;*/
}


Entity::~Entity()
{
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
	Hit(1);
}


void Entity::KillIfOut()
{
	if (outside_universe(GetRect()))
	{
		hp_ = 0;
#ifdef DEBUG_ENTITY
		sf::Vector2f pos = GetPosition();
		printf("(entity is out : x %f, y %f)\n", pos.x, pos.y);
#endif
	}
}


sf::FloatRect Entity::GetRect() const
{
	// Utiliser GetLocalRect avec SFML 1.4 (?)
	sf::FloatRect rect;
	rect.Left = GetPosition().x;
	rect.Top = GetPosition().y;
	rect.Right = rect.Left + GetSize().x;	
	rect.Bottom = rect.Top + GetSize().y;
	return rect;
}

