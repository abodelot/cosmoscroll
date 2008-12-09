#include <cstdio>

#include "Math.hpp"


namespace math
{

enum
{
	N = 314
};

static float table_sin[N];
static float table_cos[N];


void init()
{
	float angle = 0.f;
	for (int i = 0; i < N; ++i)
	{
		//printf("std::sin %.2f = %.2f\n", angle, std::sin(angle));
		table_sin[i] = std::sin(angle);
		table_cos[i] = std::cos(angle);
		angle += 0.01;
	}
	puts("math initialisé");
}

float sin(float radians)
{
	/*int rads = (int)(radians * 100) % N;
	if (rads < 0)
	{
		rads *= -1;
	}
	return radians > 0 ? table_sin[rads] : -table_sin[-rads];
	*/
	return std::sin(radians);
}

float cos(float radians)
{
	/*int rads = (int)(radians * 100) % N;
	if (rads < 0)
	{
		rads *= -1;
	}
	return table_cos[rads];*/
	return std::cos(radians);
}

}
