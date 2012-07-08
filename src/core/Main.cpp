#include <cstdio>
#include <cstring>
#include "Game.hpp"

#include "Constants.hpp"


int usage(const char *pn)
{
	const char *n = strrchr(pn, '/') + 1;	// Stick to the program name.
	printf("usage: %s [-c config_file] [-d data_path] [-h] [-v]\n\n", n);
	puts("If config_file is a directory, the game will look for a configuration file named");
	puts("\42cosmoscroll.cfg\42 (or create it if it doesn't exist).");
	puts("If it is a regular file, it will use it as an alternate configuration file.");
	puts("\nYou can currently override the game's story mode level set (MUST be a number)\n");
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
	int level_set = 0;
	std::string config_file = "";
	std::string data_dir = DEFAULT_DATA_DIR;

	// parse args
	for (int i = 0; i < argc; ++i)
	{
		std::string arg = argv[i];
		if (arg == "-v" || arg == "-version")
			return version();
		else if (arg == "-h" || arg == "-help")
			return usage(argv[0]);
		else if (arg == "-c" || arg == "-config")
			config_file = get_arg(i, argv);
		else if (arg == "-d" || arg == "-data")
			data_dir = get_arg(i, argv);
		else if (arg == "-l" || arg == "-level")
			level_set = (int)strtol(get_arg(i, argv), NULL, 10);
	}

	Game& game = Game::GetInstance();
	game.SetCurrentDirectory(argv[0]);
	if (!config_file.empty())
	{
		game.OverrideConfigFile(config_file);
	}
	game.Init(data_dir, level_set);
	return game.Run();
}
