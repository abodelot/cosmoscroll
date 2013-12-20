#ifndef ENDGAMESCENE_HPP
#define ENDGAMESCENE_HPP

#include "BaseScene.hpp"

class EntityManager;

class EndGameScene: public BaseScene
{
public:
	EndGameScene();

	void OnEvent(const sf::Event& event) override;

	void OnFocus() override;

	void Update(float frametime) override;

	void Show(sf::RenderTarget& target) const override;

private:
	void goNextScreen() const;

	sf::Clock            m_started_at;
	sf::Text             m_text;
	const EntityManager& m_entities;
};

#endif // ENDGAMESCENE_HPP

