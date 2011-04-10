#ifndef ENDGAMESCENE_HPP
#define ENDGAMESCENE_HPP

#include "BaseScene.hpp"

class EntityManager;

class EndGameScene: public BaseScene
{
public:
	EndGameScene();

	// inherited
	void OnEvent(const sf::Event& event);

	// inherited
	void Update(float frametime);

	// inherited
	void Show(sf::RenderTarget& target) const;

	// inherited
	void OnFocus();

private:
	float timer_;
	sf::String info_;
	EntityManager& entities_;
};

#endif // ENDGAMESCENE_HPP

