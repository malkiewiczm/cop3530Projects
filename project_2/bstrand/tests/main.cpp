static bool char_cmp(const char &a, const char &b)
{
	return a < b;
}

static bool char_eq(const char &a, const char &b)
{
	return a == b;
}


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../BSTRAND.h"

#define CATCH_CONFIG_MAIN

#define WHICH_MAP BSTRAND
#define WHICH_MAP_NAME "BSTRAND"
#include "test.h"
