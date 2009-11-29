#include <iostream>
#include "MediaNotFoundException.hpp"


MediaNotFoundException::MediaNotFoundException(const char* missing_file) :
	std::runtime_error(missing_file)
{
}
