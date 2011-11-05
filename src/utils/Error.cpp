#include "Error.hpp"

std::ostringstream Error::Log;


std::runtime_error Error::Exception()
{
	return Exception(Log.str());
}


std::runtime_error Error::Exception(const std::string& message)
{
	return std::runtime_error(message);
}
