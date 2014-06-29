#include "WeaponData.hpp"
#include "Weapon.hpp"
#include "core/Resources.hpp"
#include "utils/I18n.hpp"
#include "utils/tinyxml/tinyxml2.h"


WeaponData::WeaponData():
	ItemData(ItemData::WEAPON),
	m_texture(NULL)
{
	sound_ = NULL;
	heat_cost_ = 0.f;
	fire_rate_ = 1.f;
	damage_ = 1;
	speed_ = 100;
}


bool WeaponData::loadClassFromXml(tinyxml2::XMLElement* elem)
{
	const char* p = elem->Attribute("id");
	if (p == NULL)
	{
		std::cerr << "weapon id is missing (ignored)" << std::endl;
		return false;
	}
	id_ = p;

	// image
	p = elem->Attribute("image");
	if (p == NULL)
	{
		std::cerr << "weapon ammo image is missing" << std::endl;
	}
	m_texture = &Resources::getTexture(p);

	// sound (optional)
	p = elem->Attribute("sound");
	if (p != NULL)
	{
		sound_ = &Resources::getSoundBuffer(p);
	}
	return true;
}


bool WeaponData::loadFromXml(tinyxml2::XMLElement* elem)
{
	ItemData::LoadFromXml(elem); // fetch price and level

	if (elem->QueryFloatAttribute("heat_cost", &heat_cost_) != tinyxml2::XML_SUCCESS)
	{
		std::cerr << "weapon heat cost is missing" << std::endl;
	}

	if (elem->QueryFloatAttribute("fire_rate", &fire_rate_) != tinyxml2::XML_SUCCESS)
	{
		std::cerr << "weapon fire rate is missing" << std::endl;
	}

	if (elem->QueryIntAttribute("damage", &damage_) != tinyxml2::XML_SUCCESS)
	{
		std::cerr << "weapon damage is missing" << std::endl;
	}

	if (elem->QueryIntAttribute("speed", &speed_) != tinyxml2::XML_SUCCESS)
	{
		std::cerr << "weapon speed is missing" << std::endl;
	}
	return true;
}


std::wstring WeaponData::getDescription() const
{
	// Hide other properties (?)
	//wstr_self_replace(s, L"{speed}", std::to_wstring(speed_));
	//wstr_self_replace(s, L"{dmg}", std::to_wstring(damage_));
	//wstr_self_replace(s, L"{rate}", std::to_wstring((int) fire_rate_));

	return I18n::templatize("item.laser_info", "{dmg}", damage_);
}


const std::string& WeaponData::getID() const
{
	return id_;
}

