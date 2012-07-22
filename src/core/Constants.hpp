#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#define GAME_NAME					"cosmoscroll"
#define GAME_VERSION				"0.4"
#define GAME_LICENSE				"GPL"
#define GAME_AUTHORS				L"    Alexandre Bodelot\n" \
									L"    Arnaud Wolff"
#define DISCLAIMER_BLOB				"This is free software; see the source for copying conditions.  There is NO\n\
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n"

#define VERSION_STRING				GAME_NAME " " GAME_VERSION "\nCopyright (C) 2008-2012 project contributors.\n" \
									DISCLAIMER_BLOB

// fallback file paths
#define DEFAULT_DATA_DIR			"data"
#define DEFAULT_SCREENSHOT_DIR		"screenshots"

// leaderboard server
#define COSMO_SERVER_HOSTNAME "http://perkypenguin.free.fr"
#define COSMO_SERVER_URI      "cosmoscroll.php"
#define COSMO_SERVER_KEY      "secret"

// resources checksums
#define MD5SUM_ANIMATIONS "6432c2b6e1ea80b4e2dfa22b5fb5cdea"
#define MD5SUM_SPACESHIPS "9fa447de35fcbc83b092391c4ef76f3c"
#define MD5SUM_ITEMS      "a34dbee3321261eb26c23451b9eed154"

#endif // CONSTANTS_HPP
