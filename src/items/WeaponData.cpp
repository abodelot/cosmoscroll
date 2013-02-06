#include "WeaponData.hpp"
#include "Weapon.hpp"
#include "core/Resources.hpp"
#include "utils/StringUtils.hpp"
#include "utils/I18n.hpp"


WeaponData::WeaponData()
{
	image_ = NULL;
	sound_ = NULL;
	heat_cost_ = 0.f;
	fire_rate_ = 1.f;
	damage_ = 1;
	speed_ = 100;
}


bool WeaponData::LoadFromXml(tinyxml2::XMLElement* elem)
{
	ItemData::LoadFromXml(elem);

	const char* p = elem->Attribute("id");
	if (p == NULL)
	{
		std::cerr << "weapon id is missing (ignored)" << std::endl;
		return false;
	}

	id_ = p;
	SetType(ItemData::WEAPON);

	// image
	p = elem->Attribute("image");
	if (p == NULL)
	{
		std::cerr << "weapon ammo image is missing" << std::endl;
	}
	image_ = &Resources::getTexture(p);

	// sound (optional)
	p = elem->Attribute("sound");
	if (p != NULL)
	{
		sound_ = &Resources::getSoundBuffer(p);
	}

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


std::wstring WeaponData::BuildDescriptionString() const
{
	std::wstring s = _t("item.weapon_info");
	wstr_self_replace(s, L"{speed}", to_wstring(speed_));
	wstr_self_replace(s, L"{dmg}", to_wstring(damage_));
	wstr_self_replace(s, L"{rate}", to_wstring(fire_rate_));
	return s;
}


const std::string& WeaponData::GetID() const
{
	return id_;
}

