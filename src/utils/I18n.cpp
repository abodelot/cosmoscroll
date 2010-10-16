#include <clocale>
#include <cstring>
#include <fstream>
#include <iostream>

#include "I18n.hpp"
#include "StringUtils.hpp"

#define TOKEN_SEPARATOR   '='
#define TOKEN_COMMENT     '#'
#define DEFAULT_LANG_CODE "en"


I18n& I18n::GetInstance()
{
	static I18n self;
	return self;
}


I18n::I18n()
{
	code_[0] = '\0';
}


const std::wstring& I18n::Translate(const char* key) const
{
	std::map<std::string, std::wstring>::const_iterator it = content_.find(key);
	if (it == content_.end())
	{
		printf("i18n: no translation found for identifier '%s'\n", key);
		static std::wstring error;

		utf8_to_wstr(error, str_sprintf("<text '%s' missing>", key));
		return error;
	}
	return it->second;
}


bool I18n::LoadSystemLanguage()
{
	if (LoadFromCode(GetLocaleCode()))
	{
		printf("i18n: language  '%s' loaded\n", code_);
		return true;
	}
	if (LoadFromCode(DEFAULT_LANG_CODE))
	{
		printf("i18n: language '%s' not found, using default language : '%s'\n", code_, DEFAULT_LANG_CODE);
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
	strcpy(code_, locale.c_str());
	return locale;
}


const char* I18n::GetCurrentCode() const
{
	return code_;
}


bool I18n::LoadFromCode(const std::string& code)
{
	if (code.size() == 2)
	{
		std::string filename = "data/lang/" + code + ".lang";
		if (LoadFromFile(filename.c_str()))
		{
			strcpy(code_, code.c_str());
			return true;
		}
	}
	return false;
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
				str_replace(text, "\\n", "\n");
				// register wstring translation
				utf8_to_wstr(content_[key], text);
			}
		}
		return true;
	}
	return false;
}
