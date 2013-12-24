#include "UserSettings.hpp"
#include "LevelManager.hpp"
#include "Input.hpp"
#include "utils/IniParser.hpp"
#include "utils/I18n.hpp"
#include "items/ItemManager.hpp"

bool UserSettings::panel_on_top = true;

int UserSettings::s_highscore = 0;
int UserSettings::s_credits = 0;
int UserSettings::s_items[] = {0};
UserSettings::Init UserSettings::s_ctor;

UserSettings::Init::Init()
{
	for (int i = 0; i < ItemData::_COUNT; ++i)
	{
		s_items[i] = 1;
	}
}


int UserSettings::getItemLevel(ItemData::Type type)
{
	return s_items[type];
}


void UserSettings::setItemLevel(ItemData::Type type, int level)
{
	if (level < 1)
		level = 1;

	if (ItemManager::GetInstance().GetItemData(type, level) == NULL)
		level = 1;

	s_items[type] = level;
}


int UserSettings::getCredits()
{
	return s_credits;
}


void UserSettings::updateCredits(int diff)
{
	s_credits += diff;
}


int UserSettings::getHighscore()
{
	return s_highscore;
}


void UserSettings::setHighscore(int highscore)
{
	s_highscore = highscore;
}


void UserSettings::loadFromConfig(IniParser& config)
{
	config.SeekSection("Settings");
	// language
	std::string lang = config.Get("language");
	if (lang.empty() || !I18n::getInstance().loadFromCode(lang))
	{
		I18n::getInstance().loadSystemLanguage();
	}

	// Panel position
	config.Get("panel_on_top", panel_on_top);

	config.SeekSection("Player");
	int level = 1;
	config.Get("last_unlocked_level", level);
	LevelManager::getInstance().setLastUnlocked(level);

	level = 1;
	config.Get("current_level", level);
	LevelManager::getInstance().setCurrent(level);

	config.Get("credits",          s_credits);
	config.Get("arcade_highscore", s_highscore);

	config.Get("lvl_laser",    s_items[ItemData::WEAPON]);
	config.Get("lvl_shield",   s_items[ItemData::SHIELD]);
	config.Get("lvl_hull",     s_items[ItemData::HULL]);
	config.Get("lvl_engine",   s_items[ItemData::ENGINE]);
	config.Get("lvl_heatsink", s_items[ItemData::HEATSINK]);
	for (int i = 0; i < ItemData::_COUNT; ++i)
	{
		setItemLevel((ItemData::Type) i, s_items[i]);
	}

	config.SeekSection("Keyboard");
	sf::Keyboard::Key key;
	if (config.Get("move_up", key))
		Input::setKeyBinding(key, Action::UP);
	if (config.Get("move_down", key))
		Input::setKeyBinding(key, Action::DOWN);
	if (config.Get("move_left", key))
		Input::setKeyBinding(key, Action::LEFT);
	if (config.Get("move_right", key))
		Input::setKeyBinding(key, Action::RIGHT);
	if (config.Get("laser", key))
		Input::setKeyBinding(key, Action::USE_LASER);
	if (config.Get("use_cooler", key))
		Input::setKeyBinding(key, Action::USE_COOLER);
	if (config.Get("use_missile", key))
		Input::setKeyBinding(key, Action::USE_MISSILE);

	config.SeekSection("Joystick");
	unsigned int button;
	if (config.Get("pause", button))
		Input::setButtonBinding(button, Action::PAUSE);
	if (config.Get("laser", button))
		Input::setButtonBinding(button, Action::USE_LASER);
	if (config.Get("use_cooler", button))
		Input::setButtonBinding(button, Action::USE_COOLER);
	if (config.Get("use_missile", button))
		Input::setButtonBinding(button, Action::USE_MISSILE);

	int deadzone;
	if (config.Get("sensitivity", deadzone))
		Input::setJoystickDeadzone(deadzone);
}


void UserSettings::saveToConfig(IniParser& config)
{
	config.SeekSection("Settings");
	config.Set("panel_on_top", panel_on_top);
	config.Set("language", I18n::getInstance().getLangCode());

	config.SeekSection("Player");
	config.Set("current_level", LevelManager::getInstance().getCurrent());
	config.Set("last_unlocked_level", LevelManager::getInstance().getLastUnlocked());
	config.Set("credits", s_credits);
	config.Set("arcade_highscore", s_highscore);
	config.Set("lvl_laser",    s_items[ItemData::WEAPON]);
	config.Set("lvl_shield",   s_items[ItemData::SHIELD]);
	config.Set("lvl_hull",     s_items[ItemData::HULL]);
	config.Set("lvl_engine",   s_items[ItemData::ENGINE]);
	config.Set("lvl_heatsink", s_items[ItemData::HEATSINK]);

	config.SeekSection("Keyboard");
	config.Set("move_up",     Input::getKeyBinding(Action::UP));
	config.Set("move_down",   Input::getKeyBinding(Action::DOWN));
	config.Set("move_left",   Input::getKeyBinding(Action::LEFT));
	config.Set("move_right",  Input::getKeyBinding(Action::RIGHT));
	config.Set("laser",       Input::getKeyBinding(Action::USE_LASER));
	config.Set("use_cooler",  Input::getKeyBinding(Action::USE_COOLER));
	config.Set("use_missile", Input::getKeyBinding(Action::USE_MISSILE));

	config.SeekSection("Joystick");
	config.Set("pause",       Input::getButtonBinding(Action::PAUSE));
	config.Set("laser",       Input::getButtonBinding(Action::USE_LASER));
	config.Set("use_cooler",  Input::getButtonBinding(Action::USE_COOLER));
	config.Set("use_missile", Input::getButtonBinding(Action::USE_MISSILE));
	config.Set("sensitivity", Input::getJoystickDeadzone());
}
