#include <fstream>

#include "ConfigParser.hpp"
#include "StringUtils.hpp"

// éléments de la syntaxe
#define TOKEN_OPEN       '{'
#define TOKEN_CLOSE      '}'
#define TOKEN_SEPARATOR  ':'
#define TOKEN_END        ';'
#define TOKEN_COMMENT    '#'

#define INDENT          '\t'


ConfigParser::ConfigParser()
{
	cursor_ = NULL;
}


bool ConfigParser::LoadFromFile(const char* filename)
{
	std::ifstream file(filename);
	if (file)
	{
		std::string line;
		std::string content;
		while (getline(file, line))
		{
			line = str_trim(line);
			if (line.size() > 0 && line[0] != TOKEN_COMMENT)
				content += line;
		}
		file.close();
		Parse(content);
		return true;
	}
	std::cerr << "couldn't open config file: " << filename << std::endl;
	return false;
}


bool ConfigParser::SaveToFile(const char* filename) const
{
	std::ofstream file(filename);
	if (file)
	{
		// pour chaque section
		Sections::const_iterator it_sec = sections_.begin();
		for (; it_sec != sections_.end(); ++it_sec)
		{
			// nom de la section
			file << it_sec->first << '\n' << TOKEN_OPEN << '\n';
			// pour chaque élément
			const Properties& props = it_sec->second;
			Properties::const_iterator it_item = props.begin();
			for (; it_item != props.end(); ++it_item)
			{
				file << INDENT << it_item->first << TOKEN_SEPARATOR << ' '
					<< it_item->second << TOKEN_END << '\n';
			}
			file << TOKEN_CLOSE << "\n\n";
		}
		file.close();
		return true;
	}
	std::cerr << "config not saved: couldn't write " << filename << std::endl;
	return false;
}


void ConfigParser::SeekSection(const char* section)
{
	cursor_ = &sections_[section];
}


void ConfigParser::Parse(const std::string& content)
{
	bool inside_section = false;

	const int length = content.size();
	size_t token;
	for (int i = 0; i < length; ++i)
	{
		if (!inside_section)
		{
			// recherche du token de début de section
			token = content.find(TOKEN_OPEN, i);
			if (token != std::string::npos)
			{
				std::string section_name = str_trim(content.substr(i, token - i));
				cursor_ = &sections_[section_name];
				inside_section = true;
				i = token;
			}
			else
			{
				break; // fin du fichier
			}
		}
		else // sinon, on est à l'intérieur de la section
		{
			token = content.find(TOKEN_CLOSE, i);
			if (token != std::string::npos)
			{
				ParseProperties(str_trim(content.substr(i, token - i)), *cursor_);
				i = token;
			}
			else
			{
#ifdef DEBUG
				std::cerr << "missing closing symbol "<< TOKEN_CLOSE << std::endl;
#endif
				break;
			}
			// fin des propriétés, on sort de l'item
			inside_section = false;
		}
	}
}


void ConfigParser::ParseProperties(const std::string& content, Properties& props)
{
	bool at_end = true;
	const int length = content.size();
	size_t token;
	std::string current_prop;

	for (int i = 0; i < length; ++i)
	{
		if (at_end)
		{
			token = content.find(TOKEN_SEPARATOR, i);
			if (token != std::string::npos)
			{
				current_prop = str_trim(content.substr(i, token - i));
				at_end = false;
				i = token;
			}
			else
			{
				break;
			}
		}
		else
		{
			token = content.find(TOKEN_END, i);
			if (token != std::string::npos)
			{
				std::string prop_value = str_trim(content.substr(i, token - i));
				// nouvelle paire clef: valeur enregistrée
				props[current_prop] = prop_value;
				at_end = true;
				i = token;
			}
			else
			{
				break;
			}
		}
	}
}


bool ConfigParser::ReadItem(const char* item, std::string& value)
{
	if (cursor_ == NULL)
	{
		std::cerr << "ConfigParser: no section defined" << std::endl;
		return false;
	}
	Properties::const_iterator it = cursor_->find(item);
	// l'élément est-il dans la section pointée ?
	if (it == cursor_->end())
	{
#ifdef DEBUG
		std::cerr << item << " not found in current section" << std::endl;
#endif
		return false;
	}
	value = it->second;
	return true;
}

