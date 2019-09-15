#ifndef I18N_HPP
#define I18N_HPP

#include <map>
#include <string>
#include <iostream>
#include <SFML/System/String.hpp>

#include "StringUtils.hpp"

/**
 * Static class for loading languages files
 */
class I18n
{
public:
    I18n();

    void setDataPath(const std::string& path);

    /**
     * Get translated text
     * @param key: text identifier
     * @return translated string
     */
    const sf::String& translate(const std::string& key) const;

    /**
     * Get the language code currently used
     * @return 2 lower-case characters
     */
    const char* getCurrentLanguage() const;

    /**
     * Load the translation file matching the system's locale
     */
    bool loadFromLocale();

    /**
     * Load the transalation file matching a given language code
     * @param code: 2-chars string language code ("en", "fr", "de"...)
     */
    bool loadFromCode(const std::string& code);

private:
    /**
     * Load a language file
     */
    bool loadFromFile(const char* filename);

    typedef std::map<std::string, sf::String> TextMap;
    TextMap     m_content;
    char        m_code[3];
    std::string m_path;
};

extern I18n g_i18n;

/**
 * Translate with current locale
 */
inline const sf::String& _t(const std::string& key)
{
    return g_i18n.translate(key);
}

/**
 * Translate a label and remplace a template key with a given value
 * @param key: label's key
 * @param search: search templated (ex: "{name}")
 * @param value: template value
 * @return translated text with template \a search replaced by \a value
 */
template <typename T>
inline sf::String _t(const char* key, const std::string& search, T value)
{
    sf::String str = g_i18n.translate(key);
    str.replace(search, std::to_string(value));
    return str;
}

/**
 * Variant with 2 keys and 2 values
 */
template <typename T1, typename T2>
inline sf::String _t(const char* key, const std::string& search1, T1 value1, const std::string& search2, T2 value2)
{
    sf::String str = g_i18n.translate(key);
    str.replace(search1, std::to_string(value1));
    str.replace(search2, std::to_string(value2));
    return str;
}

inline std::ostream& operator<<(std::ostream& os, const sf::String& str)
{
    os << str.toAnsiString();
    return os;
}

#endif // I18N_HPP
