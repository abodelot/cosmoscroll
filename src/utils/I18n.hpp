#ifndef I18N_HPP
#define I18N_HPP

#include <map>
#include <string>
#include <iostream>
#include <SFML/System/String.hpp>

#include "StringUtils.hpp"

// macro shortcut
#define _t(key) (I18n::getInstance().translate(key))

/**
 * Static class for loading languages files
 */
class I18n
{
public:
    static I18n& getInstance();

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

    /**
     * Translate a label and remplace a template by a given value
     * @param key: label's key
     * @param search: search templated (ex: "{name}")
     * @param value: template value
     * @return translated text with template \a search replaced by \a value
     */
    template <typename T>
    static sf::String templatize(const char* key, const std::string& search, T value)
    {
        sf::String str = getInstance().translate(key);
        str.replace(search, std::to_string(value));
        return str;
    }

    template <typename T1, typename T2>
    static sf::String templatize(const char* key,
        const std::string& search1, T1 value1,
        const std::string& search2, T2 value2)
    {
        sf::String str = getInstance().translate(key);
        str.replace(search1, std::to_string(value1));
        str.replace(search2, std::to_string(value2));
        return str;
    }

private:
    I18n();
    I18n(const I18n&);

    /**
     * Load a language file
     */
    bool loadFromFile(const char* filename);

    typedef std::map<std::string, sf::String> TextMap;
    TextMap     m_content;
    char        m_code[3];
    std::string m_path;
};

inline std::ostream& operator<<(std::ostream& os, const sf::String& str)
{
    os << str.toAnsiString();
    return os;
}

#endif // I18N_HPP
