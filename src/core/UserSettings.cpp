#include "UserSettings.hpp"
#include "LevelManager.hpp"
#include "Input.hpp"
#include "SoundSystem.hpp"
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
	config.seekSection("Settings");
	// language
	std::string lang = config.get("language");
	if (lang.empty() || !I18n::getInstance().loadFromCode(lang))
	{
		I18n::getInstance().loadFromLocale();
	}

	// Panel position
	panel_on_top = config.get("panel_on_top", panel_on_top);

	config.seekSection("Player");

	int level = config.get("last_unlocked_level", 1);
	LevelManager::getInstance().setLastUnlocked(level);

	level = config.get("current_level", 1);
	LevelManager::getInstance().setCurrent(level);

	s_credits = config.get("credits", 0);
	s_highscore = config.get("arcade_highscore", 0);

	s_items[ItemData::WEAPON] =   config.get("lvl_laser", 1);
	s_items[ItemData::SHIELD] =   config.get("lvl_shield", 1);
	s_items[ItemData::HULL] =     config.get("lvl_hull", 1);
	s_items[ItemData::ENGINE] =   config.get("lvl_engine", 1);
	s_items[ItemData::HEATSINK] = config.get("lvl_heatsink", 1);
	for (int i = 0; i < ItemData::_COUNT; ++i)
	{
		setItemLevel((ItemData::Type) i, s_items[i]);
	}

	config.seekSection("Keyboard");
	Input::setKeyBinding(config.get("move_up",    Input::getKeyBinding(Action::UP)), Action::UP);
	Input::setKeyBinding(config.get("move_down",  Input::getKeyBinding(Action::DOWN)), Action::DOWN);
	Input::setKeyBinding(config.get("move_left",  Input::getKeyBinding(Action::LEFT)), Action::LEFT);
	Input::setKeyBinding(config.get("move_right", Input::getKeyBinding(Action::RIGHT)), Action::RIGHT);
	Input::setKeyBinding(config.get("laser",      Input::getKeyBinding(Action::USE_LASER)), Action::USE_LASER);
	Input::setKeyBinding(config.get("cooler",     Input::getKeyBinding(Action::USE_COOLER)), Action::USE_COOLER);
	Input::setKeyBinding(config.get("missile",    Input::getKeyBinding(Action::USE_MISSILE)), Action::USE_MISSILE);

	config.seekSection("Joystick");
	Input::setButtonBinding(config.get("pause",   Input::getButtonBinding(Action::PAUSE)), Action::PAUSE);
	Input::setButtonBinding(config.get("laser",   Input::getButtonBinding(Action::USE_LASER)), Action::USE_LASER);
	Input::setButtonBinding(config.get("cooler",  Input::getButtonBinding(Action::USE_COOLER)), Action::USE_COOLER);
	Input::setButtonBinding(config.get("missile", Input::getButtonBinding(Action::USE_MISSILE)), Action::USE_MISSILE);
	Input::setJoystickDeadzone(config.get("sensitivity", Input::getJoystickDeadzone()));

	config.seekSection("Audio");
	SoundSystem::enableMusic(config.get("enable_music", true));
	SoundSystem::setMusicVolume(config.get("music_volume", 100));
	SoundSystem::enableSound(config.get("enable_sound", true));
	SoundSystem::setSoundVolume(config.get("sound_volume", 100));
}


void UserSettings::saveToConfig(IniParser& config)
{
	config.seekSection("Settings");
	config.set("panel_on_top", panel_on_top);
	config.set("language", I18n::getInstance().getCurrentLanguage());

	config.seekSection("Player");
	config.set("current_level",       LevelManager::getInstance().getCurrent());
	config.set("last_unlocked_level", LevelManager::getInstance().getLastUnlocked());
	config.set("credits",             s_credits);
	config.set("arcade_highscore",    s_highscore);
	config.set("lvl_laser",    s_items[ItemData::WEAPON]);
	config.set("lvl_shield",   s_items[ItemData::SHIELD]);
	config.set("lvl_hull",     s_items[ItemData::HULL]);
	config.set("lvl_engine",   s_items[ItemData::ENGINE]);
	config.set("lvl_heatsink", s_items[ItemData::HEATSINK]);

	config.seekSection("Keyboard");
	config.set("move_up",    Input::getKeyBinding(Action::UP));
	config.set("move_down",  Input::getKeyBinding(Action::DOWN));
	config.set("move_left",  Input::getKeyBinding(Action::LEFT));
	config.set("move_right", Input::getKeyBinding(Action::RIGHT));
	config.set("laser",      Input::getKeyBinding(Action::USE_LASER));
	config.set("cooler",     Input::getKeyBinding(Action::USE_COOLER));
	config.set("missile",    Input::getKeyBinding(Action::USE_MISSILE));

	config.seekSection("Joystick");
	config.set("pause",       Input::getButtonBinding(Action::PAUSE));
	config.set("laser",       Input::getButtonBinding(Action::USE_LASER));
	config.set("cooler",      Input::getButtonBinding(Action::USE_COOLER));
	config.set("missile",     Input::getButtonBinding(Action::USE_MISSILE));
	config.set("sensitivity", Input::getJoystickDeadzone());

	config.seekSection("Audio");
	config.set("enable_music", SoundSystem::isMusicEnabled());
	config.set("music_volume", SoundSystem::getMusicVolume());
	config.set("enable_sound", SoundSystem::isSoundEnabled());
	config.set("sound_volume", SoundSystem::getSoundVolume());
}
