#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <map>
#include <string>
#include <SFML/Window/Event.hpp>


/**
 * Gestionnaire de configuration pour mémoriser les options
 */
class Settings
{
public:
	/**
	 * Récupérer l'instance unique
	 */
	static Settings& GetInstance();
	
	enum Key
	{
		PAUSE, UP, DOWN, LEFT, RIGHT, WEAPON1, WEAPON2, BONUS_COOLER
	};
	
	/**
	 * Obtenir le code SFML d'une touche
	 */
	 sf::Key::Code GetKey(Key key) const;
	 
	bool Fullscreen() const;

	float GetBestTime() const;

	/**
	 * Charger les options depuis fichier
	 * @param[in] filename: fichier à charger
	 * @return true si le fichier a pu être lu, sinon false
	 */
	bool Load(const char* filename);

	
private:
	Settings();
	
	/**
	 * Récupérer l'option et sa valeur d'une ligne
	 */
	void ParseLine(std::string& line);
	
	typedef std::map<Key, sf::Key::Code> OptKeys;
	mutable OptKeys keys_;
	bool fullscreen_;
	float best_time_;
};

#endif /* guard SETTINGS_HPP */

