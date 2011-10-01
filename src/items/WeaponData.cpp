#include "WeaponData.hpp"
#include "Weapon.hpp"
#include "utils/MediaManager.hpp"

WeaponData::WeaponData()
{
	image_ = NULL;
	sound_ = "";
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
	if (!sound_.empty())
	{
		weapon->SetSoundName(sound_.c_str());
	}
}


bool WeaponData::LoadFromXml(TiXmlElement* elem)
{
	const char* p = elem->Attribute("image");
	if (p == NULL)
	{
		std::cerr << "weapon ammo image is missing" << std::endl;
	}
	image_ = &MediaManager::GetInstance().GetImage(p);

	// sound (optional)
	p = elem->Attribute("sound");
	sound_ = p == NULL ? "" : p;

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
