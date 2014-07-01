#ifndef PLAYSCREEN_HPP
#define PLAYSCREEN_HPP

#include "Screen.hpp"

class EntityManager;
class ControlPanel;

/**
 * Playing state
 */
class PlayScreen: public Screen
{
public:
	PlayScreen();

	void OnEvent(const sf::Event& event) override;

	void OnFocus() override;

	void Update(float frametime) override;

	void Show(sf::RenderTarget& target) const override;

private:
	void setPanelOnTop(bool top);

	EntityManager& m_entities;
	ControlPanel&  m_panel;
};

#endif // PLAYSCREEN_HPP

