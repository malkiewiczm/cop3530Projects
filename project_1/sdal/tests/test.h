#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdexcept>
#include <iostream>
#include <deque>
#include <vector>
#include <sstream>

TEST_CASE(CLASS_NAME, "[]") {
	srand(222);
	SECTION("test values part 1") {
		std::deque<int> std_list;
		LIST_T list;
		for (int i = 0; i < 200; i++) {
			int v = rand();
			if (rand() & 1) {
				std_list.push_front(v);
				list.push_front(v);
			} else {
				std_list.push_back(v);
				list.push_back(v);
			}
		}
		for (int i = 0; i < 50; i++) {
			switch (rand() % 4) {
			case 0:
				REQUIRE(list.pop_front() == std_list.front());
				std_list.pop_front();
				break;
			case 1:
				REQUIRE(list.pop_back() == std_list.back());
				std_list.pop_back();
				break;
			case 2:
				REQUIRE(list.peek_front() == std_list.front());
				break;
			case 3:
				REQUIRE(list.peek_back() == std_list.back());
				break;
			}
		}
	}

	SECTION("test values part 2") {
		std::vector<int> std_list;
		LIST_T list;
		for (int i = 0; i < 50; i++) {
			int v = rand();
			list.push_back(v);
			std_list.push_back(v);
		}
		for (int i = 0; i < 200; i++) {
			int v = rand() % 50;
			REQUIRE(list.item_at(v) == std_list[v]);
		}
	}

	SECTION("test length()") {
		LIST_T list;
		REQUIRE(list.is_empty());
		for (int i = 0; i < 10; i++)
			list.push_back(i);
		REQUIRE(list.length() == 10);
		for (int i = 0; i < 10; i++)
			list.push_front(i);
		REQUIRE(list.length() == 20);
		for (int i = 0; i < 9; i++)
			list.insert(i, i);
		list.insert(100, 29);
		REQUIRE(list.length() == 30);
		REQUIRE(! list.is_empty());
		list.clear();
		REQUIRE(list.is_empty());
	}

	SECTION("test contains()") {
		LIST_T list;
		for (int i = 0; i < 50; i++)
			list.push_back(i);
		for (int i = 0; i < 50; i++) {
			REQUIRE(list.contains(i, int_eq));
			REQUIRE(! list.contains(i + 50, int_eq));
		}
	}

	SECTION("test print()") {
		LIST_T list;
		std::stringstream ss_1;
		list.print(ss_1);
		REQUIRE(ss_1.str() == "<empty list>");
		std::stringstream ss_2;
		for (int i = 1; i < 10; ++i) {
			list.push_back(i * 11);
		}
		list.print(ss_2);
		REQUIRE(ss_2.str() == "[11,22,33,44,55,66,77,88,99]");
		list.clear();
		std::stringstream ss_3;
		list.print(ss_3);
		REQUIRE(ss_3.str() == "<empty list>");
	}

	SECTION("test contents() and remove()") {
		LIST_T list;
		for (int i = 0; i < 50; i++)
			list.push_back(i);
		int *arr = list.contents();
		REQUIRE(arr != nullptr);
		for (int i = 0; i < 50; i++) {
			REQUIRE(arr[i] == i);
		}
		delete [] arr;
		for (int i = 0; i < 20; i++) {
			list.remove(20);
		}
		arr = list.contents();
		for (int i = 0; i < 20; i++) {
			REQUIRE(arr[i] == i);
		}
		for (int i = 20; i < 30; i++) {
			REQUIRE(arr[i] == i + 20);
		}
		delete [] arr;
		REQUIRE(list.remove(0) == 0);
		REQUIRE(list.remove(28) == 49);
	}

	SECTION("test iterator") {
		LIST_T list;
		for (int i = 0; i < 10; i++) {
			list.push_back(i);
		}
		int counter = 0;
		for (auto item : list) {
			REQUIRE(item == counter);
			++counter;
		}
		LIST_T::iterator iter = list.begin();
		LIST_T::iterator end = list.end();
		for (; iter != end; ++iter) {
			iter = 5;
		}
		for (auto item : list) {
			REQUIRE(item == 5);
		}
	}

	SECTION("test replace()") {
		LIST_T list;
		for (int i = 0; i < 10; i++) {
			list.push_back(i);
		}
		list.replace(33, 3);
		list.replace(55, 5);
		list.replace(99, 9);
		int tmp[10] = { 0, 1, 2, 33, 4, 55, 6, 7, 8, 99 };
		int i = 0;
		for (auto item : list) {
			REQUIRE(item == tmp[i]);
			++i;
		}
	}

	SECTION("test insert()") {
		LIST_T list;
		for (int i = 0; i < 10; i++) {
			list.push_back(i);
		}
		list.insert(66, 3);
		list.insert(77, 3);
		list.insert(88, 3);
		list.insert(99, 3);
		REQUIRE(list.length() == 14);
		int tmp[14] = { 0, 1, 2, 99, 88, 77, 66, 3, 4, 5, 6, 7, 8, 9 };
		int i = 0;
		for (auto item : list) {
			REQUIRE(item == tmp[i]);
			++i;
		}
		putchar('\n');
	}
}
