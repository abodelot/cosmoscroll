#ifndef DIE_HPP
#define DIE_HPP

#include <cstdlib>
#include <cstdio>


#define DIE(...) \
{\
	fprintf(stderr, "\nOoops! application aborted at line %d in file %s\n", __LINE__, __FILE__);\
	fprintf(stderr, "reason: ");\
	fprintf(stderr, __VA_ARGS__);\
	fputc('\n', stderr);\
	exit(EXIT_FAILURE);\
}

#endif // DIE_HPP
