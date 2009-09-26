#ifndef HIT_HPP
#define HIT_HPP

#include "Entity.hpp"


class Hit: public Entity
{
public:
	Hit(Entity::Team team, const sf::Vector2f& offset, float angle,
		const sf::Image* image, int speed, int damage);

	Hit* Clone() const;

	void Update(float frametime);

	void OnCollide(Entity& entity);

	void TakeDamage(int);

private:
	/*class Impact
	{
	public:
		Impact(Hit& hit) :
			hit_(hit)
		{
			radius_ = 100;
		}
		void operator()(Entity& e)
		{
			if (math::distance(e.GetPosition(), hit_.GetPosition()) < radius_)
			{
				hit_.OnCollide(e);
			}
		}

	private:
		Hit& hit_;
		int radius_;
	};*/

	float angle_;
	int speed_;
};

#endif // HIT_HPP
