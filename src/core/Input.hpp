#ifndef INPUT_HPP
#define INPUT_HPP

#include <SFML/Window.hpp>


class IniParser;

/**
 * Association des événements issus des périphériques aux actions du jeu
 */
class Input
{
public:
	enum Action
	{
		PAUSE,
		TAKE_SCREENSHOT,
		PANEL_UP,
		PANEL_DOWN,
		MOVE_UP,
		MOVE_DOWN,
		MOVE_LEFT,
		MOVE_RIGHT,
		USE_LASER,
		USE_PLASMA,
		USE_COOLER,
		USE_MISSILE,
		ENTER,
		EXIT_APP,

		COUNT // do not use
	};

	enum Device
	{
		KEYBOARD = 1 << 1,
		JOYSTICK = 1 << 2,
		ALL      = KEYBOARD | JOYSTICK
	};

	static Input& GetInstance();

	void Init(const sf::Input& sfinput);

	/**
	 * Transformer un événement en action
	 * @param event: événement à tester
	 * @return action correspondante
	 */
	Action EventToAction(const sf::Event& event);

	/**
	 * Tester si une action est entrée
	 */
	bool HasInput(Action action);

	/**
	 * Définir un binding clavier
	 * @param key: touche à attacher
	 * @param action: action à attacher
	 */
	void SetKeyboardBind(sf::Key::Code key, Action action);

	/**
	 * Obtenir un binding clavier
	 * @param action: action à tester
	 * @return touche attachée
	 */
	sf::Key::Code GetKeyboardBind(Action action);

	/**
	 * Définir un binding joystick
	 * @param joybutton: numéro du bouton à attacher
	 * @param action: action à attacher
	 */
	void SetJoystickBind(unsigned int joybutton, Action action);

	/**
	 * Obtenir un binding joystick
	 * @param action: action à tester
	 * @return numéro du bouton attaché
	 */
	unsigned int GetJoystickBind(Action action);

	/**
	 * Obtenir le nom d'une action
	 */
	static const std::wstring& ActionToString(Action action);

	/**
	 * Obtenir le nom d'une touche
	 * @param key: key code sfml de la touche
	 * @return nom (en anglais)
	 */
	static const char* KeyToString(int key);

	/**
	 * Définir les périphériques à activer
	 * @param flag: bitmask des périphériques
	 */
	void SetDevices(unsigned int flag);

	/**
	 * Saisi d'un binding par l'utilisateur
	 * @param device: périphérique à tester
	 * @param action: action dont le binding doit être modifié
	 */
	void AskUserInput(Device device, Action action);

	/**
	 * Sensibilité du contrôleur de jeu
	 */
	int GetSensitivity() const;
	void SetSensitivity(int sensitivity);

	/**
	 * Charger une configuration de bindings
	 */
	void LoadFromConfig(IniParser& config);

	/**
	 * Sauvegarder les bindings dans une configuration
	 */
	void SaveToConfig(IniParser& config) const;

private:
	Input() {};
	Input(const Input&);

	enum {MAX_JOY_BUTTON = 16};

	// keyboard bindings
	Action key_to_action_[sf::Key::Count];
	sf::Key::Code action_to_key_[COUNT];

	// joystick bindings
	Action joybutton_to_action_[MAX_JOY_BUTTON];
	unsigned int action_to_joybutton_[COUNT];

	int device_flag_;
	const sf::Input* sfinput_;
	int sensitivity_;
};


std::istream& operator>>(std::istream& in, sf::Key::Code& code);

#endif // INPUT_HPP

