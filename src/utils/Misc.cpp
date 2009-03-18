#include <sstream>
#include <cassert>

#include "Misc.hpp"

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
#ifdef DEBUG
			puts("vswprintf returned -1, allocate new buffer");
#endif
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


int find_replace(std::string& target, const std::string& look_for,
	const std::string& replace_by)
{
    int cpt = 0;
    size_t pos = 0;
    size_t step = replace_by.size();
    size_t offset = look_for.size();

    pos = target.find(look_for, pos);

    while (pos != std::string::npos)
    {
        target.replace(pos, offset, replace_by);
        pos = target.find(look_for, pos + step);
        ++cpt;
    }
    return cpt;
}


void trim(std::string& str)
{
	const char* WHITESPACES = " \t\n\r\0xb";
    std::string::size_type first = str.find_first_not_of(WHITESPACES);
    if (first != std::string::npos)
    {
        std::string::size_type last = str.find_last_not_of(WHITESPACES);
        str = str.substr(first, last - first + 1);
    }
}


std::string extract(const std::string& str, int from, int to)
{
	assert(from < to);
    std::string extracted = str.substr(from, to - from);
    trim(extracted);
    return extracted;
}

