#ifndef MEDIANOTFOUNDEXCEPTION_HPP
#define MEDIANOTFOUNDEXCEPTION_HPP

#include <stdexcept>

class MediaNotFoundException: public std::runtime_error
{
public:
	MediaNotFoundException(const char* missing_file);
};


#endif // MEDIANOTFOUNDEXCEPTION_HPP
