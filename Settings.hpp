#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <map>
#include <string>
#include <SFML/Window/Event.hpp>
#include <SFML/System.hpp>


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

	enum JoystickBind
	{		// Valid vaut aussi pour l'in-game-menu
		jPAUSE, jUP, jDOWN, jLEFT, jRIGHT, jWEAPON1, jWEAPON2, jBONUS_COOLER, jVALID, jRETURN
	};
	
	/**
	 * Obtenir le code SFML d'une touche
	 */
	sf::Key::Code GetKey(Key key) const;
	 
	unsigned int GetJoyKey(JoystickBind bind) const;

	bool Fullscreen() const;

	float GetBestTime() const;

	/**
	 * Charger les options depuis fichier
	 * @param[in] filename: fichier à charger
	 * @return true si le fichier a pu être lu, sinon false
	 */
	bool Load(const char* filename);

	inline const sf::Vector2f* GetCalibration() const
	{
		return &calibration_;
	};
	
	inline void SetCalibration(const sf::Vector2f& val)
	{
		calibration_ = val;
	};
	
private:
	Settings();
	
	/**
	 * Récupérer l'option et sa valeur d'une ligne
	 */
	void ParseLine(std::string& line);
	
	typedef std::map<Key, sf::Key::Code> OptKeys;
	typedef std::map<JoystickBind, unsigned int> OptJoyControls;
	mutable OptKeys keys_;
	mutable OptJoyControls joy_binds_;
	bool fullscreen_;
	float best_time_;
	sf::Vector2f calibration_;
};

#endif /* guard SETTINGS_HPP */

