#ifndef STRINGUTILS_HPP
#define STRINGUTILS_HPP

#include <cstdarg>
#include <string>
#include <sstream>
#include <iomanip>

/**
 * Build a string with printf formatting
 * @param format: string format
 * @param ...: arguments
 * @return string
 */
//std::string str_sprintf(const char format[], ...);
//std::wstring str_sprintf(const wchar_t format[], ...);

#define wstr_self_replace str_self_replace<std::wstring>
#define wstr_replace      str_replace<std::wstring>

/**
 * Replace all occurrences of a substring (in-place)
 * @return occurrences count
 */
//int str_self_replace(std::string& str, const std::string& look_for, const std::string& replace_by);
//int str_self_replace(std::wstring& str, const std::wstring& look_for, const std::wstring& replace_by);

template <typename T>
inline int str_self_replace(T& target, const T& look_for, const T& replace_by)
{
    int cpt = 0;
    size_t pos = target.find(look_for, 0);
    size_t step = replace_by.size();
    size_t offset = look_for.size();

    while (pos != std::string::npos)
    {
        target.replace(pos, offset, replace_by);
        pos = target.find(look_for, pos + step);
        ++cpt;
    }
    return cpt;
}

/**
 * Return a copy of the string with all occurrences of a substring replaced
 */
//std::string str_replace(const std::string& str, const std::string& look_for, const std::string& replace_by);
//std::wstring str_replace(const std::wstring& str, const std::wstring& look_for, const std::wstring& replace_by);

template <typename T>
inline T str_replace(const T& str, const T& look_for, const T& replace_by)
{
    T result = str;
    str_self_replace(result, look_for, replace_by);
    return result;
}

/**
 * Return a copy of the string with leading and trailing whitespace removed
 */
std::string str_trim(const std::string& str);
std::wstring str_trim(const std::wstring& str);

/**
 * Return a copy of the string converted to lowercase
 */
std::string str_lower(const std::string& str);
std::wstring str_lower(const std::wstring& str);

/**
 * Convert string to lowercase (in-place)
 */
void str_self_lower(std::string& str);
void str_self_lower(std::wstring& str);

/**
 * Return a copy of the string converted to uppercase
 */
std::string str_upper(const std::string& str);
std::wstring str_upper(const std::wstring& str);

/**
 * Convert string to uppercase (in-place)
 */
void str_self_upper(std::string& str);
void str_self_upper(std::wstring& str);

/// Encode to UTF-8
std::string encode_utf8(const std::wstring& src);

/// Decode from UTF-8
std::wstring decode_utf8(const std::string& src);

#endif // STRINGUTILS_HPP
