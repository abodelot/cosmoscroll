#ifndef STRINGUTILS_HPP
#define STRINGUTILS_HPP

#include <cstdarg>
#include <string>
#include <sstream>
#include <iomanip>
/**
 * Construire une std::string formatée "à la printf"
 * @param format: formatage de la chaîne
 * @param ...: arguments
 * @return string formée
 */
//std::string str_sprintf(const char format[], ...);
//std::wstring str_sprintf(const wchar_t format[], ...);

template <typename T>
inline std::string to_string(const T& t, int width = 0, char fill = '0')
{
	std::ostringstream oss;
	if (width > 0)
		oss << std::setw(width) << std::setfill(fill);
	oss  << t;
	return oss.str();
}


/**
 * Return a copy of the string with all occurrences of a substring replaced
 */
std::string str_replace(const std::string& str, const std::string& look_for, const std::string& replace_by);

/**
 * Replace all occurrences of a substring (in-place)
 * @return occurrences count
 */
int str_self_replace(std::string& str, const std::string& look_for, const std::string& replace_by);


/**
 * Return a copy of the string with leading and trailing whitespace removed
 */
std::string str_trim(const std::string& str);


/**
 * Return a copy of the string converted to lowercase
 */
std::string str_lower(const std::string& str);

/**
 * Convert string to lowercase (in-place)
 */
void str_self_lower(std::string& str);


/**
 * Return a copy of the string converted to uppercase
 */
std::string str_upper(const std::string& str);

/**
 * Convert string to uppercase (in-place)
 */
void str_self_upper(std::string& str);


#endif // STRINGUTILS_HPP
