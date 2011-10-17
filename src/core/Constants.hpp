#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#define GAME_NAME					"cosmoscroll"
#define GAME_VERSION				"0.4-devel"
#define GAME_LICENSE				"GPL"
#define GAME_AUTHORS				L"    Alexandre Bodelot\n" \
									 "    Arnaud Wolff"
#define DISCLAIMER_BLOB				"This is free software; see the source for copying conditions.  There is NO\n\
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n"

#define VERSION_STRING				GAME_NAME " " GAME_VERSION "\nCopyright (C) 2008-2011 project contributors.\n" \
									DISCLAIMER_BLOB

// fallback file paths
#define DEFAULT_CONFIGURATION_FILE	"config/cosmoscroll.cfg"
#define DEFAULT_DATA_DIR			"data"
#define DEFAULT_SCREENSHOT_DIR		"screenshot"

// score server
#define COSMO_SERVER_HOSTNAME "http://perkypenguin.free.fr"
#define COSMO_SERVER_URI      "cosmoscroll.php"
#define COSMO_SERVER_KEY      "secret"

// resources checksums
#define MD5SUM_ANIMATIONS "fe92e99e42e6cc9d7c8199b9f8bb7254"
#define MD5SUM_SPACESHIPS "47a7c66f1ecb72fdd88dd473ba835965"
#define MD5SUM_WEAPONS    "21dba0fe07f8dcab9b4b699cc8ae6ac8"


#endif // CONSTANTS_HPP
