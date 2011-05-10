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

std::string str_replace(const std::string& str, const std::string& look_for, const std::string& replace_by)
{
	std::string result = str;
	str_self_replace(result, look_for, replace_by);
	return result;
}


int str_self_replace(std::string& target, const std::string& look_for, const std::string& replace_by)
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


std::string str_trim(const std::string& str)
{
	const char* WHITESPACES = " \t\n\r\0xb";
	std::string::size_type first = str.find_first_not_of(WHITESPACES);
	if (first != std::string::npos)
	{
		std::string::size_type last = str.find_last_not_of(WHITESPACES);
		return str.substr(first, last - first + 1);
	}
	return str;
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

/*
Copyright (c) 2009 SegFault aka "ErV" (altctrlbackspace.blogspot.com)

Redistribution and use of this source code, with or without modification, is
permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright
 notice, this list of conditions and the following disclaimer.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO
EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


