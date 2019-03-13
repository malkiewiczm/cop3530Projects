static bool int_eq(const int &a, const int &b)
{
	return a == b;
}

#include "../PSLL.h"
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#define CLASS_NAME "PSLL"
#define LIST_T cop3530::PSLL<int>
#include "test.h"
