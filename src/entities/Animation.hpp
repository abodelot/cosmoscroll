#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <SFML/Graphics.hpp>
#include <vector>

class Animation {
public:
    Animation();

    /**
     * Time to wait before displaying the next frame in the animation
     * @param delay: duration in seconds
     */
    void setDelay(float delay);
    float getDelay() const;

    /**
     * Texture containing the animation frames
     */
    void setTexture(const sf::Texture& texture);
    const sf::Texture& getTexture() const;

    /**
     * Animation total duration
     */
    float getDuration() const;

    /**
     * Append a frame to the animation
     * @param subrect: frame subrect in the animation texture
     */
    void addFrame(const sf::IntRect& subrect);

    /**
     * Get a frame in the animation
     * @param index: frame's index
     * @return texture subrect
     */
    const sf::IntRect& getFrame(int index) const;

    /**
     * Get number of frames in the animation
     */
    size_t getFrameCount() const;

private:
    std::vector<sf::IntRect> m_frames;
    const sf::Texture*       m_texture;
    float                    m_delay;
};

#endif
