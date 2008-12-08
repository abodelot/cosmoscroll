#ifndef MISC_HPP
#define MISC_HPP

#include <string>
#include <cstdarg>

/*
 * Construire une std::string formatée "à la printf"
 * @param[in] format: formatage de la chaîne
 * @param[in] ...: arguments
 * @return string formée
 */
std::string str_sprintf(const char format[], ...);

std::wstring str_sprintf(const wchar_t format[], ...);

/*
 * Remplacer un motif par un autre
 * @param[in|out] target: chaîne cible
 * @param[in] look_for: sous-chaîne recherchée
 * @param[in] replace_by: sous-chaîne à caser
 * @return string transformée // FIXME: hein ? lies !
 */
int find_replace(std::string& target, const std::string& look_for,
	const std::string& replace_by);

/*
 * Supprimer les blancs en début et fin de chaîne
 * @param[in|out] str: chaînée à tronquer
 */
void trim(std::string& str);

/*
 * Conversion d'une chaîne en int
 */
int str_to_int(const std::string& str);

/*
 * Conversion d'une chaîne en bool
 */
bool str_to_bool(const std::string& str);

/*
 * Conversion d'une chaîne en float
 */
float str_to_float(const std::string& str);

/*
 * Conversion d'une chaîne en char
 */
char str_to_char(const std::string& str);


#endif /* guard MISC_HPP */

