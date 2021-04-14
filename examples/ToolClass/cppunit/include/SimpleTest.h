#include "StiBel/CppUnitDefine.h"
#include <cppunit/TestCase.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestSuite.h>
#include <string>

using std::string;

// 定义测试用例
class SimpleTest : public CppUnit::TestCase
{
public:
    SimpleTest();
    ~SimpleTest();

    void testString();
    void testNumber();

    void setUp();
    void tearDown();

    static CppUnit::Test *suite();

private:
};
