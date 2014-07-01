#ifndef INTROSCREEN_HPP
#define INTROSCREEN_HPP

#include "Screen.hpp"

class EntityManager;
class Player;

/**
 * Interactive splash screen displayed at game launch
 */
class IntroScreen: public Screen
{
public:
	IntroScreen();

	void OnEvent(const sf::Event& event) override;

	void Update(float frametime) override;

	void Show(sf::RenderTarget& target) const override;

private:
	float elapsed_;
	sf::Sprite m_background;
	sf::Sprite m_title;
	EntityManager& m_entities;
	Player* ship_;
};

#endif // INTROSCREEN_HPP

