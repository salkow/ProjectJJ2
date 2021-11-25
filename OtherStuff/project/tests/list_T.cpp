#include "../acutest.h"
#define macro TEST_NO_MAIN

#include "../classes/list_T.hpp"

namespace LIST_T {

	void push(void) {
		List<int> l;
		int myNum = 3;
		// test if push adds the item to the list
		l.push(&myNum);
		TEST_CHECK(l.at(1) == &myNum);

		// test if push adds the item to the end of the list
		int myNum2 = 5;
		l.push(&myNum2);
		TEST_CHECK(l.at(2) == &myNum2);
	}

	void push_back(void) {
		List<int> l;
		int myNum = 3;
		// test if push_back adds the item to the list
		l.push_back(&myNum);
		TEST_CHECK(l.at(1) == &myNum);

		// test if push_back adds the item to the beggining of the list
		int myNum2 = 5;
		l.push_back(&myNum2);
		TEST_CHECK(l.at(1) == &myNum2);
	}

	void indexOf(void) {
		List<int> l;
		int myNums[] = {1, 2, 3, 4, 5};
		for (int i = 0; i < 5; i++) {
			l.push(&myNums[i]);
		}

		for (int i = 1; i <= 5; i++) {
			TEST_CASE_("%d must be in position %d", i, i);
			TEST_CHECK(l.indexOf(i) == i);
		}
	}

	void at(void) {
		List<int> l;
		int myNums[] = {1, 2, 3, 4, 5};
		for (int i = 0; i < 5; i++) {
			l.push(&myNums[i]);
		}

		for (int i = 1; i <= 5; i++) {
			TEST_CASE_("%d must be myNums[%d]", i, i - 1);
			TEST_CHECK(l.at(i) == &myNums[i - 1]);
		}
	}

	void pop(void) {
		List<int> l;
		int myNums[] = {1, 2, 3, 4, 5};
		for (int i = 0; i < 5; i++) {
			l.push(&myNums[i]);
		}

		for (int i = 4; i >= 0; i--) {
			TEST_CHECK_(l.pop() == &myNums[i], "popped value must be myNums[%d]", i);
		}

		for (int i = 0; i < 5; i++) {
			l.push(&myNums[i]);
		}

		TEST_CHECK_(l.pop(2) == &myNums[1], "popped value must be myNums[%d]", 1);
		TEST_CHECK_(l.pop(1) == &myNums[0], "popped value must be myNums[%d]", 0);
		TEST_CHECK_(l.pop(3) == &myNums[4], "popped value must be myNums[%d]", 4);
	}

	void getSize(void) {
		List<int> l;
		int myNum = 3;
		TEST_CHECK(l.getSize() == 0);
		l.push(&myNum);
		TEST_CHECK(l.getSize() == 1);
		l.push(&myNum);
		TEST_CHECK(l.getSize() == 2);
		l.pop();
		TEST_CHECK(l.getSize() == 1);
		l.pop();
		TEST_CHECK(l.getSize() == 0);
	}

	void deleteContents(void) {
		List<int> l;
		int * myNumPointers[5];

		for (int i = 0; i < 5; i++) {
			myNumPointers[i] = new int(i);
			l.push(myNumPointers[i]);
		}
		l.deleteContents();

		TEST_CHECK(l.getSize() == 0);
	}

	void copyConstructor(void) {
		List<int> l;
		int myNums[] = {1, 2, 3, 4, 5};
		for (int i = 0; i < 5; i++) {
			l.push(&myNums[i]);
		}

		List<int> l2(l);

		for (int i = 1; i <= 5; i++) {
			TEST_CASE_("%d must be myNums[%d]", i, i - 1);
			TEST_CHECK(l2.at(i) == &myNums[i - 1]);
		}
	}

	void equalsOperator(void) {
		List<int> l, l2;
		int myNums[] = {1, 2, 3, 4, 5};
		for (int i = 0; i < 5; i++) {
			l.push(&myNums[i]);
		}

		l2 = l;

		for (int i = 1; i <= 5; i++) {
			TEST_CASE_("%d must be myNums[%d]", i, i - 1);
			TEST_CHECK(l2.at(i) == &myNums[i - 1]);
		}
	}

	static TEST_STRUCT tests[] = {{"List_push", LIST_T::push},
								  {"List_push_back", LIST_T::push_back},
								  {"List_getSize", LIST_T::getSize},
								  {"List_at", LIST_T::at},
								  {"List_pop", LIST_T::pop},
								  {"List_indexOf", LIST_T::indexOf},
								  {"List_deleteContents", LIST_T::deleteContents},
								  {"List_copy_constructor", LIST_T::copyConstructor},
								  {"List_operator=", LIST_T::equalsOperator},
								  {NULL, NULL}};
	TEST_STRUCT * getTests() { return tests; };
} // namespace LIST_T

#define LIST_T_TESTS
