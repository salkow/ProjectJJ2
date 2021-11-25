#define TEST_LIST_FREE
#include "acutest.h"

#include "tests/list_T.cpp"
#include "tests/BK_T.cpp"
#include "tests/dedup.cpp"
#include "tests/Elgoog.cpp"

int get_list_size(TEST_STRUCT * list) {
	int size = 0;
	for (int i = 0; list[i].func != NULL; i++)
		size++;
	return size;
}

TEST_LIST_PROVIDER() {
	TEST_STRUCT * dedup = (TEST_STRUCT *) DEDUP::getTests();
	int dedup_size		= get_list_size(dedup);

	TEST_STRUCT * bk_t = (TEST_STRUCT *) BK_T::getTests();
	int bk_t_size	   = get_list_size(bk_t);

	TEST_STRUCT * list_t = (TEST_STRUCT *) LIST_T::getTests();
	int list_t_size		 = get_list_size(list_t);

	TEST_STRUCT * elgoog = (TEST_STRUCT *) ELGOOG::getTests();
	int elgoog_size		 = get_list_size(elgoog);

	int total_size	   = dedup_size + bk_t_size + list_t_size + elgoog_size;
	TEST_STRUCT * list = (TEST_STRUCT *) malloc(sizeof(TEST_STRUCT) * (total_size + 1));

	memcpy(list, dedup, sizeof(TEST_STRUCT) * dedup_size);
	list += dedup_size;

	memcpy(list, bk_t, sizeof(TEST_STRUCT) * bk_t_size);
	list += bk_t_size;

	memcpy(list, list_t, sizeof(TEST_STRUCT) * list_t_size);
	list += list_t_size;

	memcpy(list, elgoog, sizeof(TEST_STRUCT) * elgoog_size);
	list += elgoog_size;

	list[0] = {NULL, NULL};

	return list - (total_size);
}
