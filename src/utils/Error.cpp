#include <fstream>
#include "Error.hpp"

std::ostringstream Error::log;


std::runtime_error Error::exception()
{
	return exception(log.str());
}


std::runtime_error Error::exception(const std::string& message)
{
	return std::runtime_error(message);
}


void Error::dump()
{
	std::ofstream file("error.log");
	if (file)
	{
		file << log.str();
		file.close();
	}
}
