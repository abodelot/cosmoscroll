#ifndef I18N_HPP
#define I18N_HPP

#include <map>
#include <string>

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
	const std::wstring& translate(const char* key) const;
	const std::wstring& translate(const std::string& key) const;

	/**
	 * Load the language file corresponding to the system's locale
	 * @return true if language file found
	 */
	bool loadSystemLanguage();

	/**
	 * Get language code for the loaded language file
	 * @return 2 lower-case characters
	 */
	const char* getLangCode() const;

	/**
	 * Charger un fichier de traduction particulier
	 * @param filename: nom du fichier à charger
	 */
	bool loadFromFile(const char* filename);

	bool loadFromCode(const std::string& code);

	/**
	 * Translate a label and remplace a template by a given value
	 * @param key: label's key
	 * @param search: search templated (ex: "{name}")
	 * @param value: template value
	 * @return translated text with template \a search replaced by \a value
	 */
	template <typename T>
	static std::wstring templatize(const char* key, const std::string& search, T value)
	{
		return str_replace(getInstance().translate(key),
		                   std::wstring(search.begin(), search.end()),
		                   std::to_wstring(value));
	}

private:
	I18n();
	I18n(const I18n&);

	typedef std::map<std::string, std::wstring> TextMap;
	TextMap     m_content;
	char        m_code[3];
	std::string m_path;
};

#endif // I18N_HPP
