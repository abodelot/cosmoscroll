#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP

#include <string>

/**
 * Static class for handling OS-specific paths
 */
class FileSystem
{
public:
    /**
     * @return true if path is a directory
     */
	static bool isDirectory(const std::string& path);

	/**
     * @return true if path is a regular file
     */
	static bool isFile(const std::string& path);

    /**
     * Create a directory
     * @return true if directory successfully created
     */
	static bool createDirectory(const std::string& name);

	/**
	 * Ensure settings directory is created
	 * @param appname: name of application directory
	 * @return path to settings directory
	 */
	static std::string initSettingsDirectory(const std::string& appname);
};

#endif // FILESYSTEM_HPP
