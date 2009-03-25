#include "BulletManager.hpp"
#include "../utils/MediaManager.hpp"
#include "../core/Window.hpp"
#include "../core/ParticleSystem.hpp"
#include "../utils/Math.hpp"


BulletManager& BulletManager::GetInstance()
{
	static BulletManager self;
	return self;
}


void BulletManager::Update(float frametime)
{
	BulletIterator it;
	for (it = bullets_.begin(); it != bullets_.end(); ++it)
	{
		float dist = it->speed * frametime;
		it->sprite.Move(dist * math::cos(it->angle), -dist * math::sin(it->angle));
	}
}


void BulletManager::Add(int emitter_id, const sf::Vector2f& offset, float angle,
	const sf::Image* image, int speed, int damage)
{
	Bullet bullet;

	bullet.sprite.SetImage(*image);
	bullet.damage = damage;
	bullet.speed = speed;
	bullet.sprite.SetPosition(offset);
	bullet.sprite.SetRotation(RAD_TO_DEG(angle));
	bullet.angle = angle;
	bullet.emitter_id = emitter_id;
	bullets_.push_back(bullet);
}


void BulletManager::Collide(std::list<Entity*>& entities)
{
	static sf::FloatRect window_rect(0, 0, WIN_WIDTH, WIN_HEIGHT);
	static ParticleSystem& particles = ParticleSystem::GetInstance();

	sf::FloatRect beamrect;
	BulletIterator it_b;
	// pour chaque beam
	for (it_b = bullets_.begin(); it_b != bullets_.end();)
	{
		bool dead = false; // état du beam
		const sf::Vector2f& vec_pos = it_b->sprite.GetPosition();
		const sf::Vector2f& vec_size = it_b->sprite.GetSize();
		beamrect.Left = vec_pos.x;
		beamrect.Top = vec_pos.y;
		beamrect.Right = beamrect.Left + vec_size.x;
		beamrect.Bottom = beamrect.Top + vec_size.y;
		std::list<Entity*>::iterator it_e;
		// pour chaque vaisseau
		for (it_e = entities.begin(); it_e != entities.end(); ++it_e)
		{
			// don't kill the emitter
			if (it_b->emitter_id == (**it_e).GetID())
			{
				continue;
			}
			else
			// si la position du beam est dans la surface du vaisseau
			if ((**it_e).GetCollideRect().Intersects(beamrect))
			{
				dead = true;
				(**it_e).Hit(it_b->damage);
				particles.AddImpact(sf::Vector2f(vec_pos.x, vec_pos.y), 10);
				break;
			}
		}
		// si hors de l'éran
		if (!window_rect.Contains(vec_pos.x, vec_pos.y))
		{
			dead = true;
		}
		// remove dead beams
		if (dead)
		{
			it_b = bullets_.erase(it_b);
		}
		else
		{
			++it_b;
		}
	}
}


void BulletManager::Show(sf::RenderWindow& app)
{
	BulletIterator it;
	for (it = bullets_.begin(); it != bullets_.end(); ++it)
	{
		app.Draw(it->sprite);
	}
}


void BulletManager::Clear()
{
	bullets_.clear();
}


BulletManager::BulletManager()
{
}

