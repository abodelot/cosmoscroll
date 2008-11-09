#include "Settings.hpp"
#include "Misc.hpp"

#include <iostream>
#include <fstream>

#define SEPARATOR '='
#define COMMENT   '#'


Settings& Settings::GetInstance()
{
    static Settings self;
    return self;
}


bool Settings::Fullscreen() const
{
	return fullscreen_;
}


float Settings::GetBestTime() const
{
	return best_time_;
}


sf::Key::Code Settings::GetKey(Key key) const
{
	return keys_[key];
}


bool Settings::Load(const char* filename)
{
    std::ifstream file(filename);
    if (file)
    {
        std::string line;
        while (getline(file, line))
        {
            if (line.size() == 0)
            {
#ifdef DEBUG
				puts("la ligne est vide");
#endif
				continue;
			}
			if (line[0] == COMMENT)
            {
#ifdef DEBUG
				puts("la ligne est un commentaire");
#endif
				continue;
			}
			ParseLine(line);
            size_t pos = line.find(SEPARATOR);
            if (pos != std::string::npos)
            {
            	ParseLine(line);

            }
        }
        file.close();
        return true;
    }
    std::cerr << "impossible d'ouvrir " << filename << std::endl;
    return false;
}


void Settings::ParseLine(std::string& line)
{
	// découpage de la ligne en "option SEPARATOR value"
	size_t pos = line.find(SEPARATOR);
	if (pos != std::string::npos)
	{
		std::string option = line.substr(0, pos);
		trim(option);
		std::string value = line.substr(pos + 1);
		trim(option);
		if (option == "pause")
		{
			keys_[PAUSE] = (sf::Key::Code) str_to_int(value);
		}
		else if (option == "up")
		{
			keys_[UP] = (sf::Key::Code) str_to_int(value);
		}
		else if (option == "down")
		{
			keys_[DOWN] = (sf::Key::Code) str_to_int(value);
		}
		else if (option == "left")
		{
			keys_[LEFT] = (sf::Key::Code) str_to_int(value);
		}
		else if (option == "right")
		{
			keys_[RIGHT] = (sf::Key::Code) str_to_int(value);
		}
		else if (option == "weapon1")
		{
			keys_[WEAPON1] = (sf::Key::Code) str_to_int(value);
		}
		else if (option == "weapon2")
		{
			keys_[WEAPON2] = (sf::Key::Code) str_to_int(value);
		}
		else if (option == "bonus_cooler")
		{
			keys_[BONUS_COOLER] = (sf::Key::Code) str_to_int(value);
		}
		else if (option == "best_time")
		{
			best_time_ = str_to_float(value);
		}
		else if (option == "fullscreen")
		{
			fullscreen_ = str_to_bool(value);
		}
		else
		{
			std::cerr << "option inconnue : " << option << std::endl;
		}
	}
	else
	{
		std::cerr << "ligne mal formée ignorée : " << line << std::endl;
	}
}


Settings::Settings()
{
	// valeurs par défaut
	keys_[PAUSE] = sf::Key::P;
	keys_[UP] = sf::Key::Up;
	keys_[DOWN] = sf::Key::Down;
	keys_[LEFT] = sf::Key::Left;
	keys_[RIGHT] = sf::Key::Right;
	keys_[WEAPON1] = sf::Key::Space;
	keys_[WEAPON2] = sf::Key::A;
	keys_[BONUS_COOLER] = sf::Key::C;
	
	fullscreen_ = false;
	best_time_ = 0.0f;
}

