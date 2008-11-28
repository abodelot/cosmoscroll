#include <cassert>
#include <iostream>

#include "PlayerManager.hpp"

PlayerManager& PlayerManager::GetInstance()
{
    static PlayerManager self;
    return self;
}

int PlayerManager::New()
{
	++ last_;
	std::cerr << "PlayerManager::New() " << last_ << std::endl;
	// HACK: Virer le offset obligatoire à la création?
	Player player;
	player.best_time = 0.f;
	player.ship = NULL;
	players_[last_] = player;
	current_ = last_;
	return last_;
}

void PlayerManager::Clear()
{
	PlayerIterator it;
	for (it = players_.begin(); it != players_.end(); ++it)
	{
		Delete ((*it).first);
	}
	last_ = current_ = -1;
}

void PlayerManager::Delete(int id)
{
	if (players_[id].ship != NULL)
	{
		delete players_[id].ship;
	}
	players_.erase(id);
}

void PlayerManager::Place()
{
	assert (InBounds());
	
	players_[current_].Place();
}

float PlayerManager::GetBestTime()
{
	assert (InBounds());

	return players_[current_].best_time;
}

PlayerShip* PlayerManager::GetShip()
{
	//std::cerr << "GetShip: " << current_ << ", vaut " << players_[current_].ship << "\n";
	assert (InBounds());
	//assert (players_[current_].ship != NULL);
	return players_[current_].ship;
}

PlayerShip* PlayerManager::NewShip(sf::Vector2f& offset)
{
	assert (InBounds());
	if (players_[current_].ship)
		DelShip();
	players_[current_].ship = new PlayerShip(offset);
	std::cerr << "Ds NewShip, playership = " << players_[current_].ship << "\n";
	return players_[current_].ship;
}

void PlayerManager::DelShip()
{
	assert (InBounds());

	delete players_[current_].ship;
	players_[current_].ship = NULL;
}

int PlayerManager::GetControlMode()
{
	assert (InBounds());

	return players_[current_].ctrl_mode;
}

void PlayerManager::SetBestTime(float time)
{
	assert (InBounds());

	players_[current_].best_time = time;
}

void PlayerManager::SetControlMode(int mode)
{
	assert (InBounds());

	players_[current_].ctrl_mode = mode;
	players_[current_].ship->SetControls(mode);
}

PlayerManager::PlayerManager()
{
	last_ = current_ = -1;
}


PlayerManager::~PlayerManager()
{
	Clear();
}

void PlayerManager::Select(int id)
{
	assert (InBounds());
	
	current_ = id;
}

int PlayerManager::Selected()
{
	return current_;
}
