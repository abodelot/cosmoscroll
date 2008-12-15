#include <fstream>

#include "ConfigParser.hpp"
#include "Misc.hpp"

#define OPEN_TOKEN      '{'
#define CLOSE_TOKEN     '}'
#define PROP_SEPARATOR  ':'
#define PROP_END        ';'
#define COMMENT         '#'

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
			trim(line);
			if (line.size() > 0 && line[0] != COMMENT)
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
			file << it_sec->first << '\n' << OPEN_TOKEN << '\n';
			// pour chaque élément
			const Properties& props = it_sec->second;
			Properties::const_iterator it_item = props.begin();
			for (; it_item != props.end(); ++it_item)
			{
				file << INDENT << it_item->first << PROP_SEPARATOR << ' '
					<< it_item->second << PROP_END << '\n';
			}
			file << CLOSE_TOKEN << "\n\n";
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
			token = content.find(OPEN_TOKEN, i);
			if (token != std::string::npos)
			{
				std::string section_name = extract(content, i, token);
				
				//printf("parsing section: %s\n", section_name.c_str());
				cursor_ = &sections_[section_name];
				inside_section = true;
				i = token;
			}
			else
			{
				i = length;
				puts("EOF");
			}
		}
		else // sinon, on est à l'intérieur de la section
		{
			token = content.find(CLOSE_TOKEN, i);
			if (token != std::string::npos)
			{
				ParseProperties(extract(content, i, token), *cursor_);
				i = token;
			}
			else
			{
				std::cerr << "closing bracket '}' missing" << std::endl;
				i = length;
			}
			// fin des propriétés, on sort de l'item
			inside_section = false;
		}
	}
}


void ConfigParser::ParseProperties(const std::string& content, Properties& props)
{
	enum Status
	{
		SEARCH_SEPARATOR,
		SEARCH_END_PROP
	};
	Status status = SEARCH_SEPARATOR;
	const int length = content.size();
	size_t token;
	std::string current_prop;
	
	for (int i = 0; i < length; ++i)
	{
		if (status == SEARCH_SEPARATOR)
		{
			token = content.find(PROP_SEPARATOR, i);
			if (token != std::string::npos)
			{
				current_prop = extract(content, i, token);
				//printf("add prop : %s\n", current_prop.c_str());
				status = SEARCH_END_PROP;
				i = token;
			}
			else
			{
				i = length;
			}
		}
		else if (status == SEARCH_END_PROP)
		{
			token = content.find(PROP_END, i);
			if (token != std::string::npos)
			{
				std::string prop_value = extract(content, i, token);
				// nouvelle paire clef: valeur enregistrée
				props[current_prop] = prop_value;
				
				status = SEARCH_SEPARATOR;
				i = token;
			}
			else
			{
				i = length;
			}
		}
	}
}

