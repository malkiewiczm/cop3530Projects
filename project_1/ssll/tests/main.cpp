static bool int_eq(const int &a, const int &b)
{
	return a == b;
}

#include "../SSLL.h"
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#define CLASS_NAME "SSLL"
#define LIST_T cop3530::SSLL<int>
#include "test.h"
