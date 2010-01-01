#ifndef I18N_HPP
#define I18N_HPP

#include <map>

/**
 * Singleton dédié à l'intertionalisation (dit i18n)
 */
class I18n
{
public:
	static I18n& GetInstance();

	/**
	 * Obtenir un text localisé
	 * @param key: identifiant du texte
	 */
	const std::wstring& Translate(const char* key) const;

	// raccourci syntaxique I18n::t
	inline static const std::wstring& t(const char* key)
	{ return I18n::GetInstance().Translate(key); }

	/**
	 * Tente de charger le fichier de traduction de la langue du système
	 * @return true si langue du système trouvée, sinon false
	 */
	bool LoadSystemLanguage();

	/**
	 * Obtenir le code de la langue du système
	 * @return 2 caractères minuscules
	 */
	std::string GetLocaleCode() const;

	/**
	 * Charger un fichier de traduction particulier
	 * @param filename: nom du fichier à charger
	 */
	bool LoadFromFile(const char* filename);

private:
	I18n();
	I18n(const I18n&);

	std::map<std::string, std::wstring> content_;
};

#endif // I18N_HPP

