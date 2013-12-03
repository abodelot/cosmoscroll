#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#define APP_WIDTH                   640
#define APP_HEIGHT                  480
#define APP_FPS                     100
#define APP_TITLE                   "CosmoScroll"

#define GAME_NAME					"cosmoscroll"
#define GAME_VERSION				"0.4"
#define GAME_LICENSE				"GPL"
#define GAME_AUTHORS				L"Alexandre Bodelot\n" \
									L"Arnaud Wolff"

#define VERSION_STRING				GAME_NAME " " GAME_VERSION "\n\n" \
                                    "Copyright (C) 2008-2013 project contributors.\n" \
									"This is free software; see the source for copying conditions.  There is NO\n" \
                                    "warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n"

// Default file paths
#define DEFAULT_RESOURCES_DIR       "resources"
#define DEFAULT_SCREENSHOT_DIR      "screenshots"

// Leaderboard server
#define COSMO_SERVER_HOSTNAME "http://perkypenguin.free.fr"
#define COSMO_SERVER_URI      "cosmoscroll.php"
#define COSMO_SERVER_KEY      "secret"

// XML Resources checksums
#define MD5SUM_ANIMATIONS "6432c2b6e1ea80b4e2dfa22b5fb5cdea"
#define MD5SUM_SPACESHIPS "9fa447de35fcbc83b092391c4ef76f3c"
#define MD5SUM_ITEMS      "96b220b011b29e7a45d5f671bdc27c0e"

#endif // CONSTANTS_HPP
