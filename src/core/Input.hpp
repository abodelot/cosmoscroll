#ifndef INPUT_HPP
#define INPUT_HPP

#include <map>
#include <SFML/Window.hpp>

class Action
{
public:
    enum ID
    {
        NONE,
        UP,
        DOWN,
        LEFT,
        RIGHT,
        USE_LASER,
        USE_MISSILE,
        USE_COOLER,
        TAKE_SCREENSHOT,
        PAUSE,
        EXIT_APP,
        PANEL_UP,
        PANEL_DOWN,
        VALIDATE
    };

    static const sf::String& toString(ID action);
};

class Input
{
public:
    /**
     * Give an event to process
     * @return action ID matching the event
     */
    static Action::ID feedEvent(const sf::Event& event);

    /**
     * Get action matching a given key
     * @return matching action ID, or NONE
     */
    static Action::ID matchKey(sf::Keyboard::Key key);

    /**
     * Get action matching a given joystick button
     * @return matching action ID, or NONE
     */
    static Action::ID matchButton(unsigned int button);

    /**
     * Check if the key or the button binded to an action is hold down
     */
    static bool isPressed(Action::ID action);

    /**
     * Bind a key to an action
     */
    static void setKeyBinding(sf::Keyboard::Key key, Action::ID action);

    /**
     * Get the key binded to an action
     */
    static sf::Keyboard::Key getKeyBinding(Action::ID action);

    /**
     * Bind a joystick button to an action
     */
    static void setButtonBinding(unsigned int joystick_button, Action::ID action);

    /**
     * Get the joystick button binded to an action
     */
    static unsigned int getButtonBinding(Action::ID action);

    /**
     * Set the amount the analog joystick must be tilted before triggering action
     */
    static void setJoystickDeadzone(int dead_zone);

    /**
     * Get the amount the analog joystick must be tilted before triggering action
     */
    static int getJoystickDeadzone();

    /**
     * Get string representation of a key
     */
    static const char* keyToString(sf::Keyboard::Key key);

    /**
     * Get string representation of a joystick button
     */
    static sf::String buttonToString(unsigned int button);

private:
    typedef std::map<sf::Keyboard::Key, Action::ID> KeyMap;
    typedef std::map<unsigned int, Action::ID> ButtonMap;
    typedef std::map<Action::ID, bool> ActionMap;
    static KeyMap       s_keys;
    static ButtonMap    s_buttons;
    static ActionMap    s_pressed;
    static int s_joystick_deadzone;

    static struct Init { Init(); } s_ctor; // Static ctor
};

std::istream& operator>>(std::istream& in, sf::Keyboard::Key& code);

#endif // INPUT_HPP
