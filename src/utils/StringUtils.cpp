#include "StringUtils.hpp"
#include <locale>

namespace utils {

std::string dirname(const std::string& path)
{
    size_t last_separator = path.find_last_of("/\\");
    std::string result = path.substr(0, last_separator + 1);
    return result;
}

std::string trim(const std::string& str)
{
    const char* WHITESPACES = " \t\n\r\0xb";
    std::string::size_type first = str.find_first_not_of(WHITESPACES);
    if (first != std::string::npos) {
        std::string::size_type last = str.find_last_not_of(WHITESPACES);
        return str.substr(first, last - first + 1);
    }
    return std::string();
}

std::string lower(const std::string& str)
{
    std::string result(str);
    for (size_t i = 0; i < str.length(); ++i) {
        result[i] = std::tolower(str[i]);
    }
    return result;
}

std::string upper(const std::string& str)
{
    std::string result(str);
    for (size_t i = 0; i < str.length(); ++i) {
        result[i] = std::toupper(str[i]);
    }
    return result;
}

}
