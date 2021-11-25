#include "../acutest.h"

#define macro TEST_NO_MAIN

namespace TEST_FILE_TEMPLATE {
	static void test(void) { TEST_CHECK(1 == 1); };

	static TEST_STRUCT tests[] = {{"test", test}, {NULL, NULL}};
	TEST_STRUCT * getTests() { return tests; };
} // namespace TEST_FILE_TEMPLATE
