#include <stdio.h>
#include "Game.hpp"

#include "Constants.hpp"


int usage(const char *pn)
{
	char *n = rindex(pn, '/') + 1;	// Stick to the program name.
	printf("usage: %s [-c config_file] [-l level_set] [-h] [-v]\n\n", n);
	puts("if config_file is a directory, the game will look for a configuration file");
	puts("named \42config.cfg\42. If it is a regularfile, it will use it as an alternate");
	puts("configuration file.");
	puts("\nYou can currently override the game's story mode level set (MUST be a number\n");
	return (0);
}

int version(void)
{
	return (puts(VERSION_STRING));
}

char *get_conf(int c, char **v)
{
	for (unsigned short i = 1; i < c; ++i)
		if (!strcmp(v[i], "-c"))
		{
			if (i == (c - 1))
				exit(puts("error: -c takes an argument"));
			return (v[i + 1]);
		}
	return NULL;
}

int parse_args(int c, char **v)
{
	bool vers, help;
	unsigned int ls;
	vers = help = (ls = 0);
	
	for (unsigned short i = 1; i < c; ++i)
	{
		char *p = v[i];
		
		if		(!strcmp(p, "-v"))	vers = true;
		else if (!strcmp(p, "-h"))	help = true;
		else if (!strcmp(p, "-l"))
		{	// next line is sloppy behaviour.
			if (i == (c - 1)) exit(puts("error: -l takes an argument"));
			ls = (int)strtol(v[++i], NULL, 10);			
		}
	}
	if (vers) version();
	if (help) usage(*v);
	if (vers || help) exit(EXIT_SUCCESS);
	return (ls);
}

int main(int argc, char* argv[])
{
	short int ls;
	char *conf_dir;
	
	ls = parse_args(argc, argv);
	conf_dir = get_conf(argc, argv);
	
	Game& game = Game::GetInstance();
	game.Init(conf_dir? conf_dir : DEFAULT_CONFIGURATION_DIR, ls);
	return (game.Run());
}

