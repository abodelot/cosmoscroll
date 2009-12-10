#ifndef BASESCENE_HPP
#define BASESCENE_HPP

#include <SFML/Graphics.hpp>

/**
 * Base de toutes les scènes affichées à l'écran
 */
class BaseScene
{
public:
	/**
	 * Réagir à un événement
	 */
	virtual void OnEvent(const sf::Event& event) = 0;

	/**
	 * Mettre la scène à jour
	 * @param frametime: temps de la frame courante
	 */
	virtual void Update(float frametime) {}

	/**
	 * Afficher la scène et tous ses éléments
	 * @param target: cible de rendu pour l'affichage
	 */
	virtual void Show(sf::RenderTarget& target) const = 0;

	/**
	 * Notifier la scène qu'elle est la scène courante
	 */
	virtual void Poke() {}
};

#endif // BASESCENE_HPP
