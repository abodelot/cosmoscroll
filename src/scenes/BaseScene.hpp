#ifndef BASESCENE_HPP
#define BASESCENE_HPP

#include <SFML/Graphics.hpp>

/**
 * Base interface for scenes
 */
class BaseScene
{
public:
	virtual ~BaseScene() {}

	/**
	 * On event callback
	 */
	virtual void OnEvent(const sf::Event& event) = 0;

	/**
	 * On focus callback (this scene became the current one)
	 */
	virtual void OnFocus() {}

	/**
	 * Update scene
	 * @param frametime: time elapsed in the current frame
	 */
	virtual void Update(float frametime) { (void) frametime; }

	/**
	 * Display scene
	 */
	virtual void Show(sf::RenderTarget& target) const = 0;
};

#endif // BASESCENE_HPP
