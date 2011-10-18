#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP

#include <string>

/**
 * Static class for handling OS-specific paths
 */
class FileSystem
{
public:
	static bool IsDirectory(const std::string& path);
	
	static bool CreateDirectory(const std::string& path);

	/**
	 * Ensure settings directory is created
	 * @param appname: name of application directory
	 * @return path to settings directory
	 */
	static std::string InitSettingsDirectory(const std::string& appname);
};

#endif // FILESYSTEM_HPP