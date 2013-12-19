#ifndef BASEMENU_HPP
#define BASEMENU_HPP

#include <vector>
#include <string>
#include <SFML/Graphics.hpp>

#include "BaseScene.hpp"
#include "CosmoButton.hpp"
#include "../gui/gui.hpp"

/**
 * Menu navigable pour sélectionner un choix
 */
class BaseMenu: public BaseScene, public gui::Menu
{
public:
	BaseMenu();

	void OnEvent(const sf::Event& event) override;

	void OnFocus() override;

	void Update(float frametime) override;

	void Show(sf::RenderTarget& target) const override;

protected:
	/**
	 * Indiquer le titre du menu (aucun par défaut)
	 * @param text: contenu du titre
	 * @param y: décalage axe Y
	 */
	void SetTitle(const sf::String& text, int y = 20);
	const sf::Text& GetTitle() const;

	void OnWidgetFocused() override;

private:
	xsf::Text title_;
	mutable xsf::Sprite scrolling_background_;
	static gui::WidgetStyle global_style_;
};

#endif // BASEMENU_HPP

