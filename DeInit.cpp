//DeInit.cpp
#include "DeInit.h"


int DeInit()
{
	gluDeleteQuadric(quadratic);
	KillFont();

	return TRUE;
}

