#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <vector>
#include <SFML/Graphics.hpp>


class Animation
{
public:
	Animation();

	/**
	 * Définir le temps d'attente entre chaque frame
	 * @param delay: temps en secondes
	 */
	void setDelay(float delay);

	/**
	 * Obtenir le temps d'attente entre chaque frame
	 * @return temps en secondes
	 */
	inline float getDelay() const	{ return m_delay; }

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
	 * Obtenir une frame de l'animation
	 * @param num_frame: numéro de la frame demandée
	 * @return rectangle de la frame
	 */
	const sf::IntRect& getFrame(int num_frame) const;

	/**
	 * Obtenir le nombre de frame qui composent l'animation
	 */
	inline int getSize() const { return m_frames.size(); }

    // Texture bind to the animation
	void setTexture(const sf::Texture& image);
	const sf::Texture& getTexture() const;

private:
	std::vector<sf::IntRect> m_frames;
	const sf::Texture*       m_texture;
	float                    m_delay;
};

#endif // ANIMATION_HPP

