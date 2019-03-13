static bool int_eq(const int &a, const int &b)
{
	return a == b;
}

#include "../CBL.h"
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#define CLASS_NAME "CBL"
#define LIST_T cop3530::CBL<int>
#include "test.h"
