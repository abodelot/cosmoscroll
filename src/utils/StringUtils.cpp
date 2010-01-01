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


int str_replace(std::string& target, const std::string& look_for,
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


int str_replace(std::string& target, char look_for, char replace_by)
{
	int count = 0;
	size_t pos = 0;

	pos = target.find(look_for, pos);
	while (pos != std::string::npos)
	{
		target.replace(pos, 1, 1, replace_by);
		pos = target.find(look_for, pos);
		++count;
	}
	return count;
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


std::string str_extract(const std::string& str, int from, int to)
{
	assert(from < to);
	return str.substr(from, to - from);
}


void str_lower(std::string& str)
{
	for (size_t i = 0; i < str.length(); ++i)
	{
		str[i] = tolower(str[i]);
	}
}


void str_upper(std::string& str)
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

void utf8_to_wstr(std::wstring& dest, const std::string& src)
{
	dest.clear();
	wchar_t w = 0;
	int bytes = 0;
	wchar_t err = L'�';
	for (size_t i = 0; i < src.size(); i++){
		unsigned char c = (unsigned char)src[i];
		if (c <= 0x7f){//first byte
			if (bytes){
				dest.push_back(err);
				bytes = 0;
			}
			dest.push_back((wchar_t)c);
		}
		else if (c <= 0xbf){//second/third/etc byte
			if (bytes){
				w = ((w << 6)|(c & 0x3f));
				bytes--;
				if (bytes == 0)
					dest.push_back(w);
			}
			else
				dest.push_back(err);
		}
		else if (c <= 0xdf){//2byte sequence start
			bytes = 1;
			w = c & 0x1f;
		}
		else if (c <= 0xef){//3byte sequence start
			bytes = 2;
			w = c & 0x0f;
		}
		else if (c <= 0xf7){//3byte sequence start
			bytes = 3;
			w = c & 0x07;
		}
		else{
			dest.push_back(err);
			bytes = 0;
		}
	}
	if (bytes)
		dest.push_back(err);
}


void wstr_to_utf8(std::string& dest, const std::wstring& src)
{
	dest.clear();
	for (size_t i = 0; i < src.size(); i++){
		wchar_t w = src[i];
		if (w <= 0x7f)
			dest.push_back((char)w);
		else if (w <= 0x7ff){
			dest.push_back(0xc0 | ((w >> 6)& 0x1f));
			dest.push_back(0x80| (w & 0x3f));
		}
		else if (w <= 0xffff){
			dest.push_back(0xe0 | ((w >> 12)& 0x0f));
		dest.push_back(0x80| ((w >> 6) & 0x3f));
		dest.push_back(0x80| (w & 0x3f));
		}
		else if (w <= 0x10ffff){
			dest.push_back(0xf0 | ((w >> 18)& 0x07));
			dest.push_back(0x80| ((w >> 12) & 0x3f));
			dest.push_back(0x80| ((w >> 6) & 0x3f));
			dest.push_back(0x80| (w & 0x3f));
		}
		else
			dest.push_back('?');
	}
}
