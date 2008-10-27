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


/*
 * @param[in|out] target: chaîne cible
 * @param[in] look_for: sous-chaîne recherchée
 * @param[in] replace_by: sous-chaîne à caser
 * @return string transformée
 */
int find_replace(std::string& target, const std::string& look_for, const std::string& replace_by);
#endif /* guard MISC_HPP */

