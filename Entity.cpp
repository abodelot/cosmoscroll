#include <typeinfo>

#include "Entity.hpp"
#include "Bonus.hpp"
#include "Window.hpp"


Entity::Entity(const sf::Image& img, const sf::Vector2f& offset, int hp)
{
	sprite_.SetImage(img);
	sprite_.SetPosition(offset);
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
	Hit(1);
}


void Entity::KillIfOut()
{
	if (outside_universe(GetRect()))
	{
		hp_ = 0;
#ifdef DEBUG_ENTITY
		sf::Vector2f pos = sprite_.GetPosition();
		printf("(entity is out : x %f, y %f)\n", pos.x, pos.y);
#endif
	}
}


sf::FloatRect Entity::GetRect() const
{
	// Utiliser GetLocalRect avec SFML 1.4 (?)
	sf::FloatRect rect;
	rect.Left = sprite_.GetPosition().x;
	rect.Top = sprite_.GetPosition().y;
	rect.Right = rect.Left + sprite_.GetSize().x;	
	rect.Bottom = rect.Top + sprite_.GetSize().y;
	return rect;
}

