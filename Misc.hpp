#ifndef MISC_HPP
#define MISC_HPP

#include <string>
#include <cstdarg>

/*
 * @param[in] format: formatage de la chaîne
 * @param[in] ...: arguments
 * @return string formée
 */
std::string epic_sprintf(const char format[], ...);

#endif /* guard MISC_HPP */

