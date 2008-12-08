#include "Misc.hpp"

#include <sstream>

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

/*
std::wstring str_sprintf(const wchar_t format[], ...)
{
    va_list args;
    va_start(args, format);
    
    int length = _vsnwprintf(NULL, 0, format, args);
		
    wchar_t* p = new wchar_t [length + 1];
    va_end(args);
    va_start(args, format);
    vswprintf(p, format, args);
    va_end(args);    
    
    std::wstring str(p);
    delete [] p;
    return str;
}
*/

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
			puts("vswprintf returned -1, allocate new buffer");
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

/*
std::wstring wstr_sprintf(const char format[], ...)
{
    va_list args;
    va_start(args, format);
    
    int length = vsnprintf(NULL, 0, format, args);
    char* p = new char [length + 1];
    va_end(args);
    va_start(args, format);
    vsprintf(p, format, args);
    va_end(args);    
    
	wchar_t* w = new wchar_t [length + 1];
	
	for (unsigned short register i = 0; i < length + 1; ++i)
	{
		w[i] = btowc(p[i]);
	}
	
    std::wstring str(w);
    delete [] p;
	delete [] w;
    return str;
}
*/

int find_replace(std::string& target, const std::string& look_for,
	const std::string& replace_by)
{
    short unsigned cpt = 0;
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


int str_to_int(const std::string& str)
{
	std::istringstream iss(str);
	int i;
	iss >> i;
	return i;
}


bool str_to_bool(const std::string& str)
{
	std::stringstream ss(str);
	ss << std::boolalpha;
	bool b;
	ss >> b;
	return b;
}


float str_to_float(const std::string& str)
{
	std::istringstream iss(str);
	float f;
	iss >> f;
	return f;
}


char str_to_char(const std::string& str)
{
	return str[0];
}

