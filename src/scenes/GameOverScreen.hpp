#ifndef GAMEOVERSCREEN_HPP
#define GAMEOVERSCREEN_HPP

#include "Screen.hpp"

class EntityManager;
class ControlPanel;

class GameOverScreen: public Screen
{
public:
	GameOverScreen();

	void OnEvent(const sf::Event& event) override;

	void OnFocus() override;

	void Update(float frametime) override;

	void Show(sf::RenderTarget& target) const override;

private:
	void goNextScreen() const;

	sf::Clock            m_started_at;
	sf::Text             m_text;
	const EntityManager& m_entities;
	const ControlPanel&  m_panel;
};

#endif // GAMEOVERSCREEN_HPP

