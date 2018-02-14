/**
 *
 * Minimalistic Unit Testing Framework
 *
 */

#pragma once

#include <auxiliary/logger.h>

#include <functional>
#include <chrono>
#include <deque>

class Test {
    public:
        static void expression(const char* exp, bool passed, const char* module, int line) {
            Log::testf("%s(%d): %s: '%s'", module, line, passed ? "passed" : "failed", exp);
        }
};

struct TestInfo
{
	std::string name;
	std::function<void(void)> fn;

	TestInfo(const std::string& name, std::function<void(void)> fn) {
		this->name = name;
		this->fn = fn;
	}
};

class TestSuite {

    public:
        static int addTest(const std::string& name,
                           const std::function<void(void)>& testfn,
                           std::deque<TestInfo>* tests) {
            if (nullptr == tests) return 0;
			TestInfo ti(name, testfn);
			tests->push_back(ti);
            return (int)tests->size();
        }

        static void runTests(const std::deque<TestInfo>* tests,
                             const std::string& filter) {
            if (nullptr == tests) return;

            std::string f = filter;

			int testIdx = 0;

            for (auto& it : *tests)
            {
				testIdx++;

                std::string testName = it.name;

                if (f.empty() || 0 == f.compare("*") || 0 == f.compare(testName)) {
                    Log::testf("Running test \"%s\" (%d of %d)", testName.c_str(), testIdx, tests->size());
					auto tStart = std::chrono::high_resolution_clock::now();
					it.fn();
					auto tElapsed = std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - tStart).count();
					Log::testf("Test \"%s\" : Test execution time was %0.3f seconds.\n", testName.c_str(), tElapsed);

                } else {
                    Log::testf("Skipped test \"%s\" (%d of %d)\n", testName.c_str(), testIdx, tests->size());
                }
            }
        }
};

#define testAssert(exp)          Test::expression((#exp), (exp), __FILE__, __LINE__)
#define testAssertEqual(a,b)     Test::expression((#a "==" #b), (a)==(b), __FILE__, __LINE__)
#define testAssertTrue(a)        testAssertEqual(a, true)
#define testAssertFalse(a)       testAssertEqual(a, false)
#define testAssertNull(a)        testAssertEqual(a, nullptr)
#define testFail(str)            Test::expression((str), false, __FILE__, __LINE__)

#define IMPLEMENT_TESTRUNNER() \
std::deque<TestInfo>* __ptr_global_test_list = nullptr; \
int __global_test_list_add(const std::string& name, const std::function<void(void)>& testfn) \
{ \
    if (nullptr == __ptr_global_test_list) __ptr_global_test_list = new std::deque<TestInfo>(); \
    return TestSuite::addTest(name, testfn, __ptr_global_test_list); \
}
#define IMPLEMENT_TEST(fn) \
extern int __global_test_list_add(const std::string& name, const std::function<void(void)>& testfn); \
extern void __testfn_ ## fn();  \
auto __testfn_sym_ ## fn = __global_test_list_add(#fn, [] { __testfn_ ## fn(); }); \
void __testfn_ ## fn()

#define NOT_IMPLEMENT_TEST(fn) \
void fn()

#define RUN_TESTS(filter) TestSuite::runTests(__ptr_global_test_list, filter);
