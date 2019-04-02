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
     * The screen received an event
     */
    virtual void onEvent(const sf::Event& event) = 0;

    /**
     * The screen is the current one
     */
    virtual void onFocus() {}

    /**
     * Update screen
     * @param frametime: time elapsed in the current frame
     */
    virtual void update(float frametime) = 0;

    /**
     * Display screen on render target
     */
    virtual void draw(sf::RenderTarget& target) const = 0;
};

#endif // SCREEN_HPP
