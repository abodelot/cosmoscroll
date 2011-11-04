#include <iostream>
#include <fstream>

#include "IniParser.hpp"

#define TOKEN_COMMENT       ';'
#define TOKEN_START_SECTION '['
#define TOKEN_END_SECTION   ']'
#define TOKEN_SEPARATOR     '='


IniParser::IniParser()
{
	cursor_ = NULL;
}


bool IniParser::LoadFromFile(const std::string& filename)
{
	std::ifstream file(filename.c_str());
	if (file)
	{
		std::string line;
		Section* section = NULL;
		while (std::getline(file, line))
		{
			ClearString(line);
			if (line.size() > 0)
			{
				// look for sections
				if (line[0] == TOKEN_START_SECTION)
				{
					// extract section name
					size_t index = line.find(TOKEN_END_SECTION);
					line = line.substr(1, index - 1);
					section = &entries_[line];
				}
				else if (section != NULL)
				{
					size_t index = line.find(TOKEN_SEPARATOR);
					if (index != std::string::npos)
					{
						std::string key = line.substr(0, index);
						ClearString(key);
						std::string value = line.substr(index + 1);
						ClearString(value);
						(*section)[key] = value;
					}
				}
			}
		}
		file.close();
		return true;
	}
	std::cerr << "cannot open file: " << filename << std::endl;
	return false;
}


bool IniParser::SaveToFile(const std::string& filename)
{
	std::ofstream file(filename.c_str());
	if (file)
	{
		// loop over entries
		for (Entry::const_iterator it = entries_.begin(); it != entries_.end(); ++it)
		{
			file << TOKEN_START_SECTION << it->first << TOKEN_END_SECTION << std::endl;
			const Section& section = it->second;
			// loop over sections
			for (Section::const_iterator it2 = section.begin(); it2 != section.end(); ++it2)
			{
				file << it2->first << " " << TOKEN_SEPARATOR << " " << it2->second << std::endl;
			}
			file << std::endl;
		}
		file.close();
		return true;
	}
	std::cerr << "cannot open file: " << filename << std::endl;
	return false;
}


void IniParser::SeekSection(const std::string& section_name)
{
	cursor_ = &entries_[section_name];
}


const std::string& IniParser::Get(const std::string& key) const
{
	static std::string empty = "";
	if (cursor_ != NULL)
	{
		return (*cursor_)[key];
	}
	return empty;
}


void IniParser::ClearString(std::string& str) const
{
	if (str.size() > 0 && str[0] == TOKEN_COMMENT)
	{
		str.clear();
		return;
	}

	const char* WHITESPACES = " \t\n\r\0xb";
	size_t first = str.find_first_not_of(WHITESPACES);
	if (first != std::string::npos)
	{
		size_t last = str.find_last_not_of(WHITESPACES);
		str = str.substr(first, last - first + 1);
	}
}
