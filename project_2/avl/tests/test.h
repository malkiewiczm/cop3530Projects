#include "catch.hpp"

TEST_CASE(WHICH_MAP_NAME ": keys being inserted and removed", WHICH_MAP_NAME) {
	using map_t = cop3530::WHICH_MAP<char, int, char_cmp, char_eq>;
	map_t map;
	
	REQUIRE(map.is_empty());
	REQUIRE(map.size() == 0);
		
	REQUIRE_NOTHROW(map.insert('m', 1));
	REQUIRE(! map.is_empty());
	REQUIRE(map.size() == 1);
			
	REQUIRE_NOTHROW(map.insert('q', 2));
	REQUIRE(! map.is_empty());
	REQUIRE(map.size() == 2);
			
	REQUIRE_NOTHROW(map.insert('r', 3));
	REQUIRE(! map.is_empty());
	REQUIRE(map.size() == 3);
			
	REQUIRE_NOTHROW(map.insert('p', 4));
	REQUIRE(! map.is_empty());
	REQUIRE(map.size() == 4);
			
	REQUIRE_NOTHROW(map.insert('a', 5));

	REQUIRE(! map.is_empty());
	REQUIRE(map.size() == 5);

	REQUIRE(map.lookup('r') == 3);
	REQUIRE(map.lookup('q') == 2);
	REQUIRE(map.lookup('m') == 1);
	REQUIRE(map.lookup('a') == 5);
	REQUIRE(map.lookup('p') == 4);
	
	REQUIRE_NOTHROW(map.insert('a', 20));
	REQUIRE(map.lookup('a') == 20);

	REQUIRE_NOTHROW(map.remove('m'));
	REQUIRE_NOTHROW(map.remove('p'));

	REQUIRE(map.lookup('q') == 2);
	REQUIRE(map.lookup('r') == 3);
	REQUIRE(map.lookup('a') == 20);
			
	REQUIRE_THROWS(map.lookup('m'));
	REQUIRE_THROWS(map.lookup('p'));

	REQUIRE_THROWS(map.remove('m'));
	REQUIRE_THROWS(map.remove('p'));

}

TEST_CASE(WHICH_MAP_NAME ": testing contains and clear", WHICH_MAP_NAME) {
	using map_t = cop3530::WHICH_MAP<char, int, char_cmp, char_eq>;
	map_t map;
	
	REQUIRE_NOTHROW(map.insert('a', 'a'));
	REQUIRE_NOTHROW(map.insert('b', 'b'));
	REQUIRE_NOTHROW(map.insert('c', 'c'));

	REQUIRE(map.contains('a'));
	REQUIRE(map.contains('b'));
	REQUIRE(map.contains('c'));
	REQUIRE(! map.contains('d'));

	REQUIRE_NOTHROW(map.clear());
	REQUIRE(map.size() == 0);
	REQUIRE(map.is_empty());
}

TEST_CASE(WHICH_MAP_NAME ": copy assignment", WHICH_MAP_NAME) {
	using map_t = cop3530::WHICH_MAP<char, int, char_cmp, char_eq>;
	map_t map_a;

	REQUIRE_NOTHROW(map_a.insert('a', 51));
	REQUIRE_NOTHROW(map_a.insert('b', 52));
	REQUIRE_NOTHROW(map_a.insert('c', 53));
	REQUIRE_NOTHROW(map_a.insert('d', 54));
	REQUIRE_NOTHROW(map_a.insert('e', 55));

	map_t map_b(map_a);

	REQUIRE(map_b.lookup('a') == 51);
	REQUIRE(map_b.lookup('b') == 52);
	REQUIRE(map_b.lookup('c') == 53);
	REQUIRE(map_b.lookup('d') == 54);
	REQUIRE(map_b.lookup('e') == 55);

	map_t map_c(map_b);
	
	REQUIRE_NOTHROW(map_b.remove('c'));
	REQUIRE_NOTHROW(map_b.remove('d'));

	REQUIRE(map_c.lookup('a') == 51);
	REQUIRE(map_c.lookup('b') == 52);
	REQUIRE(map_c.lookup('c') == 53);
	REQUIRE(map_c.lookup('d') == 54);
	REQUIRE(map_c.lookup('e') == 55);
}
