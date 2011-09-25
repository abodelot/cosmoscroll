#include <sstream>
#include <cstdio>
#include <cassert>

#include "StringUtils.hpp"

#ifdef __MINGW32__
#define vswprintf _vsnwprintf
#endif


std::string str_sprintf(const char format[], ...)
{
	va_list args;
	va_start(args, format);

	int length = vsnprintf(NULL, 0, format, args);
	char* p = new char [length + 1];
	va_end(args);
	va_start(args, format);
	vsprintf(p, format, args);
	va_end(args);

	std::string str(p);
	delete [] p;
	return str;
}


std::wstring str_sprintf(const wchar_t format[], ...)
{
	int length = -1;
	wchar_t* buffer = NULL;
	va_list args;
	va_start(args, format);
	for (int buffer_size = 32; length == -1; buffer_size *= 2)
	{
		buffer = new wchar_t [buffer_size];
		length = vswprintf(buffer, buffer_size, format, args);
		if (length == -1)
		{
			delete [] buffer;
		}
	}
	va_end(args);
	va_start(args, format);
	vswprintf(buffer, length - 1, format, args);
	va_end(args);

	std::wstring str(buffer);
	delete [] buffer;
	return str;
}




std::string str_trim(const std::string& str)
{
	const char* WHITESPACES = " \t\n\r\0xb";
	std::string::size_type first = str.find_first_not_of(WHITESPACES);
	if (first != std::string::npos)
	{
		std::string::size_type last = str.find_last_not_of(WHITESPACES);
		return str.substr(first, last - first + 1);
	}
	return std::string();
}


std::string str_lower(const std::string& str)
{
	std::string result = str;
	str_self_lower(result);
	return result;
}


void str_self_lower(std::string& str)
{
	for (size_t i = 0; i < str.length(); ++i)
	{
		str[i] = tolower(str[i]);
	}
}


std::string str_upper(const std::string& str)
{
	std::string result = str;
	str_self_upper(result);
	return result;
}


void str_self_upper(std::string& str)
{
	for (size_t i = 0; i < str.length(); ++i)
	{
		str[i] = toupper(str[i]);
	}
}


