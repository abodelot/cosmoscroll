#ifndef SCREEN_HPP
#define SCREEN_HPP

#include <SFML/Graphics.hpp>

/**
 * Base interface for screens
 */
class Screen
{
public:
	virtual ~Screen() {}

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

#endif // SCREEN_HPP
