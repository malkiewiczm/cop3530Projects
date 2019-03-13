static bool int_eq(const int &a, const int &b)
{
	return a == b;
}

#include "../SDAL.h"
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#define CLASS_NAME "SDAL"
#define LIST_T cop3530::SDAL<int>
#include "test.h"
