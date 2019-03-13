static bool char_cmp(const char &a, const char &b)
{
	return a < b;
}

static bool char_eq(const char &a, const char &b)
{
	return a == b;
}


#include "../AVL.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define CATCH_CONFIG_MAIN

#define WHICH_MAP AVL
#define WHICH_MAP_NAME "AVL"
#include "test.h"

// special test case for AVL trees only;; looks at the balance factor
// to ensure it is always AVL balanced
TEST_CASE(WHICH_MAP_NAME ": balance tests for AVL trees", WHICH_MAP_NAME) {
	using map_t = cop3530::WHICH_MAP<char, int, char_cmp, char_eq>;
	map_t map;

	srand(222);

	for (int i = 0; i < 100; ++i) {
		REQUIRE_NOTHROW(map.insert((char)(rand() % 100), rand()));
		int bf = map.balance();
		bool avl_balanced = bf == 0 || bf == -1 || bf == 1;
		REQUIRE(avl_balanced);
	}
	for (char c = 1; c <= 100; ++c) {
		REQUIRE_NOTHROW(map.insert(c, c));
		int bf = map.balance();
		bool avl_balanced = bf == 0 || bf == -1 || bf == 1;
		REQUIRE(avl_balanced);
	}
	for (char c = 1; c <= 100; ++c) {
		REQUIRE(map.lookup(c) == c);
	}
}
