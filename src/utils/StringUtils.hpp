#ifndef STRINGUTILS_HPP
#define STRINGUTILS_HPP

#include <string>

namespace utils {

/**
 * Return directory path from path if any, otherwise empty
 */
std::string dirname(const std::string& path);

/**
 * Return a copy of the string with leading and trailing whitespace removed
 */
std::string trim(const std::string& str);

/**
 * Return a copy of the string converted to lowercase
 */
std::string lower(const std::string& str);

/**
 * Return a copy of the string converted to uppercase
 */
std::string upper(const std::string& str);

}

#endif // STRINGUTILS_HPP
