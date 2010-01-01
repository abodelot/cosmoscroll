#include <clocale>
#include <fstream>
#include <iostream>

#include "I18n.hpp"
#include "StringUtils.hpp"

#define TOKEN_SEPARATOR '='
#define TOKEN_COMMENT   '#'
#define DEFAULT_LANG_FILE "data/lang/en.lang"


I18n& I18n::GetInstance()
{
	static I18n self;
	return self;
}


I18n::I18n()
{
	LoadSystemLanguage();
}


const std::wstring& I18n::Translate(const char* key) const
{
	std::map<std::string, std::wstring>::const_iterator it = content_.find(key);
	if (it == content_.end())
	{
		printf("i18n: no translation found for identifier '%s'\n", key);
		static const std::wstring FAIL_STRING = L"<translation missing>";
		return FAIL_STRING;
	}
	return it->second;
}


bool I18n::LoadSystemLanguage()
{
	std::string code = GetLocaleCode();
	std::string filename = "data/lang/" + code + ".lang";
	if (LoadFromFile(filename.c_str()))
	{
		printf("i18n: language file '%s' loaded\n", filename.c_str());
		return true;
	}
	if (LoadFromFile(DEFAULT_LANG_FILE))
	{
		printf("i18n: language file '%s' not found, using default language : '%s'\n", filename.c_str(), DEFAULT_LANG_FILE);
		return true;
	}
	puts("i18n: warning, couldn't load any language file!");
	return false;
}


std::string I18n::GetLocaleCode() const
{
	std::string locale(setlocale(LC_ALL, ""));
	locale = locale.substr(0, 2);
	str_lower(locale);
	return locale;
}


bool I18n::LoadFromFile(const char* filename)
{
	printf("i18n: loading %s...\n", filename);
	std::ifstream file(filename);
	if (file)
	{
		content_.clear();
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
			if (pos == std::string::npos)
			{
				printf("i18n: parse error at line %d (%s)\n", line_number, line.c_str());
			}
			else
			{
				std::string key = str_trim(line.substr(0, pos));
				std::string text = str_trim(line.substr(pos + 1));
				str_replace(text, std::string("\\n"), std::string("\n"));
				std::wstring wtext;
				utf8_to_wstr(wtext, text);
				content_[key] = wtext;
			}
		}
		return true;
	}
	return false;
}
