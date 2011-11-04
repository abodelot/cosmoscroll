#ifndef INIPARSER_HPP
#define INIPARSER_HPP

/**
 * Simple INI file parser
 * @author Alexandre Bodelot <alexandre.bodelot@gmail.com>
 * @date 3 November 2011
 */

#include <string>
#include <map>
#include <sstream>

/**
 * Read and write INI files
 */
class IniParser
{
public:
	IniParser();

	/**
	 * Load an INI document
	 * @param filename: document filename
	 * @return true if document loaded
	 */
	bool LoadFromFile(const std::string& filename);

	/**
	 * Save INI document to a file
	 * @return true if document saved
	 */
	bool SaveToFile(const std::string& filename);

	/**
	 * Set internal cursor on a section
	 * The section will be created if it doesn't exist yet.
	 */
	void SeekSection(const std::string& section_name);

	/**
	 * Read a value from the current section
	 * @return value (or empty string if key is not found)
	 */
	const std::string& Get(const std::string& key) const;

	/**
	 * Read a value from the current section
	 * @param default_value: returned value if key is not found
	 */
	/*template <class T>
	T Get(const std::string& key, const T& default_value) const;*/
	// alt syntax
	template <class T>
	bool Get(const std::string& key, T& output_value) const;

	/**
	 * Write a value
	 */
	template <class T>
	void Set(const std::string& key, const T& value);

private:
	void ClearString(std::string& line) const;

	typedef std::map<std::string, std::string> Section;
	typedef std::map<std::string, Section> Entry;

	Entry entries_;
	Section* cursor_;
};

/*
template <class T>
T IniParser::Get(const std::string& key, const T& default_value) const
{
	std::istringstream iss(Get(key));
	T value;
	if (iss >> value)
		return value;
	return default_value;
}
*/

template <class T>
bool IniParser::Get(const std::string& key, T& value) const
{
	std::istringstream iss(Get(key));
	if (!iss.str().empty())
	{
		iss >> value;
		return true;
	}
	return false;
}


template <class T>
void IniParser::Set(const std::string& key, const T& value)
{
	if (cursor_ != NULL)
	{
		std::ostringstream oss;
		oss << value;
		(*cursor_)[key] = oss.str();
	}
}

#endif // INIPARSER_HPP
