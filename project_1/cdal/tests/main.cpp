static bool int_eq(const int &a, const int &b)
{
	return a == b;
}

#include "../CDAL.h"
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#define CLASS_NAME "CDAL"
#define LIST_T cop3530::CDAL<int>
#include "test.h"
