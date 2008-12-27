#ifndef ANIMATIONMANAGER_HPP
#define ANIMATIONMANAGER_HPP

#include <vector>

#include <SFML/Graphics.hpp>


class AnimationManager
{
public:
	/**
	 * Récupérer l'instance unique
	 */
	static AnimationManager& GetInstance();
	
	enum Animated
	{
		CAPSULE, COUNT_ANIMATED
	};
	
	struct Animation
	{
		std::vector<sf::IntRect> subrects;
		float delay;
	};
	
	const Animation& GetAnimation(Animated anim) const;
	
private:
	AnimationManager();
	AnimationManager(const AnimationManager& other);
	
	
	
	Animation animations_[COUNT_ANIMATED];
};

#endif /* ANIMATIONMANAGER_HPP */

