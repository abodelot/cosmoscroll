#include <clocale>
#include <fstream>
#include <iostream>

#include "I18n.hpp"

#define TOKEN_SEPARATOR   '='
#define TOKEN_COMMENT     '#'
#define DEFAULT_LANG_CODE "en"


I18n& I18n::getInstance()
{
	static I18n self;
	return self;
}


I18n::I18n()
{
	for (int i = 0; i < 3; ++i)
		m_code[i] = '\0';
}


void I18n::setDataPath(const std::string& path)
{
	m_path = path;
}


const std::wstring& I18n::translate(const std::string& key) const
{
	TextMap::const_iterator it = m_content.find(key);
	if (it == m_content.end())
	{
		std::cerr << "[I18n] no translation found for key '" << key << "' (" << m_code << ")" << std::endl;
		static std::wstring error(L"text not found");
		return error;
	}
	return it->second;
}


const char* I18n::getCurrentLanguage() const
{
	return m_code;
}


bool I18n::loadFromLocale()
{
	// Get system's locale, keep only the first two characters and make them lower-case
	// It's somewhat cross-platform! ("fr_Fr" => "fr", "French" => "fr")
	std::string locale(setlocale(LC_ALL, ""));
	locale = str_lower(locale.substr(0, 2));

	// Reset locale so other functions are not impacted
	setlocale(LC_ALL, "C");

	if (loadFromCode(locale))
		return true;

	if (loadFromCode(DEFAULT_LANG_CODE))
		return true;

	std::cerr <<  "[I18n] warning: couldn't load any language!" << std::endl;
	return false;
}





bool I18n::loadFromCode(const std::string& code)
{
	if (code.size() == 2 && code[0] >= 'a' && code[0] <= 'z' && code[1] >= 'a' && code[1] <= 'z')
	{
		std::string filename = m_path + "/" + code + ".lang";
		if (loadFromFile(filename.c_str()))
		{
			m_code[0] = code[0];
			m_code[1] = code[1];
			std::cout << "[I18n] language '" << code << "' loaded" << std::endl;
			return true;
		}
		std::cerr << "[I18n] no translation file found for code '" << code << "'" << std::endl;
		return false;
	}
	std::cerr << "[I18n] '" << code << "' is not a valid language code" << std::endl;
	return false;
}


bool I18n::loadFromFile(const char* filename)
{
	std::ifstream file(filename);
	if (file)
	{
		m_content.clear();
		std::string line;
		int line_number = 0;
		while (std::getline(file, line))
		{
			++line_number;
			if (line.empty() || line[0] == TOKEN_COMMENT)
			{
				continue;
			}
			size_t pos = line.find(TOKEN_SEPARATOR);
			if (pos != std::string::npos)
			{
				std::string key = str_trim(line.substr(0, pos));
				std::string text = str_trim(line.substr(pos + 1));
				str_self_replace<std::string>(text, "\\n", "\n");

				// Decode utf-8 and convert to wide string
				m_content[key] = decode_utf8(text);
			}
			else
			{
				std::cerr << "[I18n] error at line " << line_number << ": " << line << std::endl;
			}
		}
		return true;
	}
	return false;
}


