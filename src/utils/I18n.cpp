#include <clocale>
#include <cstring>
#include <fstream>
#include <iostream>

#include "I18n.hpp"
#include "StringUtils.hpp"

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
	// Get system's locale
	std::string locale(setlocale(LC_ALL, ""));
	// Only the two first lower-case characters, so it's cross platform! (fr_Fr => fr, French => fr)
	locale = str_lower(locale.substr(0, 2));
	strcpy(m_code, locale.c_str());
	// Reset locale
	setlocale(LC_ALL, "C");
}


void I18n::setDataPath(const std::string& path)
{
	m_path = path;
}


const std::wstring& I18n::translate(const char* key) const
{
	TextMap::const_iterator it = m_content.find(key);
	if (it == m_content.end())
	{
		std::cerr << "[I18n] no translation found for key \"" << key << "\" (" << m_code << ")" << std::endl;
		static std::wstring error(L"key not found");
		return error;
	}
	return it->second;
}


const std::wstring& I18n::translate(const std::string& key) const
{
	return translate(key.c_str());
}


bool I18n::loadSystemLanguage()
{
	if (loadFromCode(m_code))
	{
		std::cout << "[I18n] language '" << m_code << "' loaded" << std::endl;
		return true;
	}
	std::cout << "[I18n] language '" << m_code << "' not found" << std::endl;
	if (loadFromCode(DEFAULT_LANG_CODE))
	{
		std::cout << "[I18n] default language " << DEFAULT_LANG_CODE << " << loaded" << std::endl;
		return true;
	}
	std::cerr <<  "[I18n] warning: couldn't load any language!" << std::endl;
	return false;
}


const char* I18n::getLangCode() const
{
	return m_code;
}


bool I18n::loadFromCode(const std::string& code)
{
	if (code.size() == 2)
	{
		std::string filename = m_path + "/" + code + ".lang";
		if (loadFromFile(filename.c_str()))
		{
			strcpy(m_code, code.c_str());
			return true;
		}
	}
	else
	{
		std::cerr << "[I18n] code " << code << " is invalid" << std::endl;
	}
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

				// convert to wide string
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


