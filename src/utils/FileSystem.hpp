#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP

#include <string>

/**
 * Functions for handling OS-specific paths
 */
namespace filesystem {

/**
 * @return true if path is a directory
 */
bool is_directory(const std::string& path);

/**
 * @return true if path is a regular file
 */
bool is_file(const std::string& path);

/**
 * Create a directory
 * @return true if directory successfully created
 */
bool create_directory(const std::string& name);

/**
 * Ensure settings directory is created
 * @param appname: name of application directory
 * @return path to settings directory
 */
std::string init_settings_directory(const std::string& appname);

}

#endif // FILESYSTEM_HPP
