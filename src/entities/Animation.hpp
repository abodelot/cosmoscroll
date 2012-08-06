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
	void SetDelay(float delay);

	/**
	 * Obtenir le temps d'attente entre chaque frame
	 * @return temps en secondes
	 */
	inline float GetDelay() const
	{ return delay_; }

	float GetDuration() const;

	/**
	 * Ajouter une frame dans l'animation
	 * @param subrect: rectangle de la frame
	 */
	void AddFrame(const sf::IntRect& subrect);
	void AddFrame(int left, int up, int width, int height);

	/**
	 * Obtenir une frame de l'animation
	 * @param num_frame: numéro de la frame demandée
	 * @return rectangle de la frame
	 */
	inline const sf::IntRect& GetFrame(int num_frame) const
	{ return subrects_[num_frame]; }

	/**
	 * Obtenir le nombre de frame qui composent l'animation
	 */
	inline int GetSize() const
	{ return subrects_.size(); }

	void setTexture(const sf::Texture& image);

	const sf::Texture& getTexture() const;

private:
	std::vector<sf::IntRect> subrects_;
	const sf::Texture* image_;
	float delay_;
};

#endif // ANIMATION_HPP

