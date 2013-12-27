#ifndef INTROLEVELSCENE_HPP
#define INTROLEVELSCENE_HPP

#include "scenes/BaseScene.hpp"

/**
 * Scène d'introduction avant le début d'un niveau
 */
class IntroLevelScene: public BaseScene
{
public:
	IntroLevelScene();

	void OnEvent(const sf::Event& event) override;

	void OnFocus() override;

	void Show(sf::RenderTarget& target) const override;

private:
	sf::Sprite m_background;
	sf::Sprite m_title;
	sf::Text   m_level_text;
	sf::Text   m_quote_text;
};

#endif // INTROLEVELSCENE_HPP

