#include <cstdio>
#include <cstring>
#include "Game.hpp"

#include "Constants.hpp"


int usage(const char *pn)
{
	// Stick to the program name
	const char* n = strrchr(pn, '/'); // Unix systems

	if (n == NULL)
		n = strrchr(pn, '\\'); // Windows systems

	printf("usage: %s [-c config_file] [-r resources_dir] [-h] [-v]\n\n", n == NULL ? pn : n + 1);
	puts("If config_file is a directory, the game will look for a configuration file named");
	puts("\42cosmoscroll.ini\42 (or create it if it doesn't exist).");
	puts("If it is a regular file, it will use it as an alternate configuration file.");
	return EXIT_SUCCESS;
}


int version(void)
{
	puts(VERSION_STRING);
	return EXIT_SUCCESS;
}


const char* get_arg(int index, char** args)
{
	const char* arg_value = args[index + 1];
	if (arg_value == NULL)
	{
		fprintf(stderr, "option %s takes an argument\n", args[index]);
		exit(EXIT_FAILURE);
	}
	return arg_value;
}


int main(int argc, char* argv[])
{
	// default values
	std::string config_file = "";
	std::string res_dir = DEFAULT_RESOURCES_DIR;

	// parse args
	for (int i = 0; i < argc; ++i)
	{
		std::string arg = argv[i];
		if (arg == "-v" || arg == "--version")
			return version();
		else if (arg == "-h" || arg == "--help")
			return usage(argv[0]);
		else if (arg == "-c" || arg == "--config")
			config_file = get_arg(i, argv);
		else if (arg == "-r" || arg == "--resources")
			res_dir = get_arg(i, argv);
	}

	Game& game = Game::getInstance();
	game.init(argv[0]);
	if (!config_file.empty())
	{
		game.setConfigFile(config_file);
	}
	game.loadResources(res_dir);
	game.loadConfig();
	return game.run();
}
