#include "WeaponData.hpp"
#include "Weapon.hpp"
#include "utils/MediaManager.hpp"
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


void WeaponData::InitWeapon(Weapon* weapon) const
{
	weapon->SetImage(image_);
	weapon->SetFireRate(fire_rate_);
	weapon->SetHeatCost(heat_cost_);
	weapon->SetDamage(damage_);
	weapon->SetVelociy(speed_);
	weapon->SetSound(sound_);
}


bool WeaponData::LoadFromXml(TiXmlElement* elem)
{
	ItemData::LoadFromXml(elem);

	const char* p = elem->Attribute("id");
	if (p == NULL)
	{
		std::cerr << "weapon id is missing (ignored)" << std::endl;
		return false;
	}

	id_ = p;
	// TODO: torcheballe
	if (id_ == "hellfire")
		SetType(ItemData::LASER1);
	else if (id_ == "laser-blue")
		SetType(ItemData::LASER2);

	// image
	p = elem->Attribute("image");
	if (p == NULL)
	{
		std::cerr << "weapon ammo image is missing" << std::endl;
	}
	image_ = &MediaManager::GetInstance().GetImage(p);

	// sound (optional)
	p = elem->Attribute("sound");
	if (p != NULL)
	{
		sound_ = &MediaManager::GetInstance().GetSoundBuffer(p);
	}

	if (elem->QueryFloatAttribute("heat_cost", &heat_cost_) != TIXML_SUCCESS)
	{
		std::cerr << "weapon heat cost is missing" << std::endl;
	}

	if (elem->QueryFloatAttribute("fire_rate", &fire_rate_) != TIXML_SUCCESS)
	{
		std::cerr << "weapon fire rate is missing" << std::endl;
	}

	if (elem->QueryIntAttribute("damage", &damage_) != TIXML_SUCCESS)
	{
		std::cerr << "weapon damage is missing" << std::endl;
	}

	if (elem->QueryIntAttribute("speed", &speed_) != TIXML_SUCCESS)
	{
		std::cerr << "weapon speed is missing" << std::endl;
	}
	return true;
}


std::wstring WeaponData::BuildDescriptionString(bool include_price) const
{
	std::wstring s = _t("item.weapon_info");
	if (include_price)
	{
		s = s + L"\n" + _t("item.price");
	}
	wstr_self_replace(s, L"{speed}", to_wstring(speed_));
	wstr_self_replace(s, L"{dmg}", to_wstring(damage_));
	wstr_self_replace(s, L"{rate}", to_wstring(fire_rate_));
	wstr_self_replace(s, L"{price}", to_wstring(GetPrice()));
	return s;
}


const std::string& WeaponData::GetID() const
{
	return id_;
}

