#include <SFML/System.hpp>

#include "Asteroid.hpp"
#include "EntityManager.hpp"
#include "../core/ParticleSystem.hpp"
#include "../utils/MediaManager.hpp"
#include "../utils/Math.hpp"

// division en morceaux de taille moindre
#define BIG_SPLIT_INTO     2
#define MEDIUM_SPLIT_INTO  4

#define BASE_SPEED         80
#define ROTATION_SPEED_MIN 40
#define ROTATION_SPEED_MAX 160


Asteroid::Asteroid(const sf::Vector2f& position, Size size, float angle) :
	Entity(position, size + 1)
{
	rotation_speed_ = sf::Randomizer::Random(ROTATION_SPEED_MIN, ROTATION_SPEED_MAX);
	size_ = size;
	SetRandomImage();

	// calcul du vecteur vitesse à partir de l'angle et de la vitesse
	angle = math::deg_to_rad(angle);
	speed_.x = std::cos(angle) * BASE_SPEED;
	speed_.y = -std::sin(angle) * BASE_SPEED;

	// origin is located at sprite center to enable centered rotation
	const sf::IntRect& rect = GetSubRect();
	SetCenter(rect.GetWidth() / 2, rect.GetHeight() / 2);
	// hack: décalage de la position car l'origine du sprite est le centre et non le coin top-left
	sf::Sprite::Move(sf::Sprite::GetCenter());
}


Asteroid* Asteroid::Clone() const
{
	Asteroid* asteroid = new Asteroid(*this);
	asteroid->SetRandomImage();
	return asteroid;
}


void Asteroid::Update(float frametime)
{
	sf::Vector2f pos = GetPosition();
	pos.x += speed_.x * frametime;
	pos.y += speed_.y * frametime;
	SetPosition(pos);
	Rotate(rotation_speed_ * frametime);
}


void Asteroid::OnDestroy()
{
	sf::Vector2f pos = GetPosition();

	static EntityManager& manager = EntityManager::GetInstance();
	switch (size_)
	{
		case BIG:
			for (int i = 0; i < BIG_SPLIT_INTO; ++i)
			{
				manager.AddEntity(new Asteroid(pos, MEDIUM, sf::Randomizer::Random(0, 360)));
			}
			break;
		case MEDIUM:
			for (int i = 0; i < MEDIUM_SPLIT_INTO; ++i)
			{
				manager.AddEntity(new Asteroid(pos, SMALL, sf::Randomizer::Random(0, 360)));
			}
			break;
		default:
			break;
	}
	ParticleSystem::GetInstance().AddImpact(GetPosition(), 10);
}


void Asteroid::SetRandomImage()
{
	int x = 0;
	switch (size_)
	{
		case SMALL:
			SetImage(GET_IMG("entities/asteroid-small"));
			x = sf::Randomizer::Random(0, 3) * 16; // 4 sprites
			SetSubRect(sf::IntRect(x, 0, x + 16, 16));
			break;
		case MEDIUM:
			SetImage(GET_IMG("entities/asteroid-medium"));
			x = sf::Randomizer::Random(0, 2) * 32; // 3 sprites
			SetSubRect(sf::IntRect(x, 0, x + 32, 32));
			break;
		case BIG:
			SetImage(GET_IMG("entities/asteroid-big"));
			x = sf::Randomizer::Random(0, 2) * 48; // 3 sprites
			SetSubRect(sf::IntRect(x, 0, x + 48, 48));
			break;
	}
}


