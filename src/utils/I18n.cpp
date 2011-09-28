#include <clocale>
#include <cstring>
#include <fstream>
#include <iostream>

#include "I18n.hpp"
#include "StringUtils.hpp"

#define TOKEN_SEPARATOR   '='
#define TOKEN_COMMENT     '#'
#define DEFAULT_LANG_CODE "en"

#define DIR_LANG "data/lang/"


I18n& I18n::GetInstance()
{
	static I18n self;
	return self;
}


I18n::I18n()
{
	code_[0] = '\0';
}


const std::wstring& I18n::Translate(const char* key) const
{
	TextMap::const_iterator it = content_.find(key);
	if (it == content_.end())
	{
		std::cerr << "[I18n] no translation found for key " << key << std::endl;
		static std::wstring error(L"key not found");
		return error;
	}
	return it->second;
}


const std::wstring& I18n::Translate(const std::string& key) const
{
	return Translate(key.c_str());
}


bool I18n::LoadSystemLanguage()
{
	if (LoadFromCode(GetLocaleCode()))
	{
		std::cout << "[I18n] language '" << code_ << "' loaded" << std::endl;
		return true;
	}
	if (LoadFromCode(DEFAULT_LANG_CODE))
	{
		std::cout << "[I18n] language '" << code_ << "' not found, using default language: " << DEFAULT_LANG_CODE << std::endl;
		return true;
	}
	std::cerr <<  "[I18n] warning: couldn't load any language!" << std::endl;
	return false;
}


std::string I18n::GetLocaleCode() const
{
	std::string locale(setlocale(LC_ALL, ""));
	locale = locale.substr(0, 2);
	str_self_lower(locale);
	strcpy(code_, locale.c_str());
	return locale;
}


const char* I18n::GetCurrentCode() const
{
	return code_;
}


bool I18n::LoadFromCode(const std::string& code)
{
	if (code.size() == 2)
	{
		std::string filename = DIR_LANG + code + ".lang";
		if (LoadFromFile(filename.c_str()))
		{
			strcpy(code_, code.c_str());
			return true;
		}
	}
	return false;
}


bool I18n::LoadFromFile(const char* filename)
{
	std::ifstream file(filename);
	if (file)
	{
		content_.clear();
		std::string line;
		int line_number = 0;
		while (std::getline(file, line))
		{
			++line_number;
			if (line.empty() || line[0] == TOKEN_COMMENT)
			{
				continue;
			}
			size_t pos = line.find(TOKEN_SEPARATOR);
			if (pos != std::string::npos)
			{
				std::string key = str_trim(line.substr(0, pos));
				std::string text = str_trim(line.substr(pos + 1));
				str_self_replace<std::string>(text, "\\n", "\n");

				// convert to wide string
				content_[key] = I18n::DecodeUTF8(text);
			}
			else
			{
				std::cerr << "[I18n] error at line " << line_number << ": " << line << std::endl;
			}
		}
		return true;
	}
	return false;
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

void I18n::EncodeUTF8(const std::wstring& src, std::string& dest)
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


std::wstring I18n::DecodeUTF8(const std::string& src)
{
	std::wstring dest;
	wchar_t w = 0;
	int bytes = 0;
	wchar_t err = L'�';
	for (size_t i = 0; i < src.size(); ++i) {
		unsigned char c = (unsigned char) src[i];
		if (c <= 0x7f) {//first byte
			if (bytes) {
				dest.push_back(err);
				bytes = 0;
			}
			dest.push_back((wchar_t)c);
		}
		else if (c <= 0xbf) {//second/third/etc byte
			if (bytes) {
				w = ((w << 6)|(c & 0x3f));
				bytes--;
				if (bytes == 0)
					dest.push_back(w);
			}
			else
				dest.push_back(err);
		}
		else if (c <= 0xdf) {//2byte sequence start
			bytes = 1;
			w = c & 0x1f;
		}
		else if (c <= 0xef) {//3byte sequence start
			bytes = 2;
			w = c & 0x0f;
		}
		else if (c <= 0xf7) {//3byte sequence start
			bytes = 3;
			w = c & 0x07;
		}
		else {
			dest.push_back(err);
			bytes = 0;
		}
	}
	if (bytes)
		dest.push_back(err);
	return dest;
}
