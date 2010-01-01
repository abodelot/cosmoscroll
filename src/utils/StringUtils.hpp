#ifndef STRINGUTILS_HPP
#define STRINGUTILS_HPP

#include <string>
#include <cstdarg>

/**
 * Construire une std::string formatée "à la printf"
 * @param format: formatage de la chaîne
 * @param ...: arguments
 * @return string formée
 */
std::string str_sprintf(const char format[], ...);
std::wstring str_sprintf(const wchar_t format[], ...);

/**
 * Remplacer un motif par un autre
 * @param target: chaîne cible
 * @param look_for: sous-chaîne recherchée
 * @param replace_by: sous-chaîne à caser
 * @return nombre d'occurences remplacées
 */
int str_replace(std::string& target, const std::string& look_for, const std::string& replace_by);
int str_replace(std::string& target, char look_for, char replace_by);

/**
 * Supprimer les blancs en début et fin de chaîne
 * @param str: chaînée à tronquer
 * @return chaîne tronquée
 */
std::string str_trim(const std::string& str);

/**
 * Extraire une sous-chaîne
 * @param str: chaîne cible
 * @param from: indice de début d'extraction
 * @param to: indice de fin d'extraction
 * @return chaîne extraite
 * @pre from < to
 */
std::string str_extract(const std::string& str, int from, int to);

/**
 * Convertir une chaîne en minuscules
 */
void str_lower(std::string& str);

/**
 * Convertir une chaîne en majuscules
 */
void str_upper(std::string& str);

/**
 * Convertion UTF-8 <-> wstring
 */
void utf8_to_wstr(std::wstring& dest, const std::string& src);
void wstr_to_utf8(std::string& dest, const std::wstring& src);

#endif // STRINGUTILS_HPP
