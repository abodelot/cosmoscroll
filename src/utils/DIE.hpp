#ifndef DIE_HPP
#define DIE_HPP

#include <cstdlib>
#include <cstdio>


#define DIE(...) \
{\
	fprintf(stdout, "\nOoops! application aborted at line %d in file %s\n", __LINE__, __FILE__);\
	fprintf(stdout, "reason: ");\
	fprintf(stdout, __VA_ARGS__);\
	fputc('\n', stdout);\
	exit(EXIT_FAILURE);\
}

#endif // DIE_HPP
