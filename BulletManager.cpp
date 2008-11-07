#include "BulletManager.hpp"
#include "MediaManager.hpp"
#include "Window.hpp"
#include "ParticleSystem.hpp"
#include "PlayerShip.hpp"
#include "Math.hpp"


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
        it->sprite.Move(dist * std::cos(it->angle), -dist * std::sin(it->angle));
    }
}


void BulletManager::Add(Weapon::Type type, Entity* sender, const sf::Vector2f& offset, float angle)
{
    Bullet bullet;
    switch (type)
    {
        case Weapon::LASERBEAM:
            bullet.sprite.SetImage(GET_IMG("ammo_laserbeam"));
            bullet.damage = 2;
            bullet.speed = 360;
            break;
        case Weapon::HELLFIRE:
            bullet.sprite.SetImage(GET_IMG("ammo_hellfire"));
            bullet.damage = 1;
            bullet.speed = 480;
            break;
        case Weapon::PLASMACANNON:
            bullet.sprite.SetImage(GET_IMG("ammo_plasma"));
            bullet.damage = 1;
            bullet.speed = 420;
            break;
		case Weapon::DEVILSEYES:
			bullet.sprite.SetImage(GET_IMG("ammo_devil"));
			bullet.damage = 3;
			bullet.speed = 500;
        default:
            break;
    }
#ifndef NO_AUDIO
    sounds_[type].Play();
#endif
    bullet.sprite.SetPosition(offset);
    bullet.sprite.SetRotation(RAD_TO_DEG(angle));
    bullet.angle = angle;
    bullet.owner = sender;
    bullets_.push_back(bullet);
}


void BulletManager::Collide(std::vector<Entity*>& entities)
{
    static sf::FloatRect window_rect(0, 0, WIN_WIDTH, WIN_HEIGHT);
    
    BulletIterator it_b;
    // pour chaque beam
    for (it_b = bullets_.begin(); it_b != bullets_.end();)
    {
        bool dead = false; // état du beam
        float beam_x = it_b->sprite.GetPosition().x;
        float beam_y = it_b->sprite.GetPosition().y;
        
        std::vector<Entity*>::iterator it_e;
        // pour chaque vaisseau
        for (it_e = entities.begin(); it_e != entities.end(); ++it_e)
        {
			// La collision se fait entre le bullet et l'entité qui possède l'arme qui l'a tirée
			if (((*it_b).owner) && ((*it_b).owner == (*it_e)))
			{
				continue;
			}
			else
            // si la position du beam est dans la surface du vaisseau
            if ((*it_e)->GetRect().Contains(beam_x, beam_y))
            {
                dead = true;
                (*it_e)->Hit(it_b->damage);
                break;
            }
        }
        // si hors de l'éran
        if (!window_rect.Contains(beam_x, beam_y))
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
#ifndef NO_AUDIO
    sounds_[Weapon::LASERBEAM].SetBuffer(GET_SOUNDBUF("blaster-shot-1"));
    sounds_[Weapon::HELLFIRE].SetBuffer(GET_SOUNDBUF("blaster-shot-2"));
    //sounds_[Weapon::PLASMACANNON]
#endif
}

void BulletManager::CleanSenders(Entity* target)
{
	BulletIterator it;
	
	for (it = bullets_.begin(); it != bullets_.end(); ++it)
	{
		if (it->owner == target)
		{
			it->owner = NULL;
		}
	}
}

