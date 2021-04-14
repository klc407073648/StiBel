#include "SimpleTest.h"

SimpleTest::SimpleTest()
{
}

SimpleTest::~SimpleTest()
{
}

void SimpleTest::testString()
{
    CPPUNIT_ASSERT_EQUAL("hello", "world");
}

void SimpleTest::testNumber()
{
    CPPUNIT_ASSERT_EQUAL(1, 1);
}

void SimpleTest::setUp()
{
}

void SimpleTest::tearDown()
{
}

CppUnit::Test *SimpleTest::suite()
{
    CppUnit::TestSuite *pSuite = new CppUnit::TestSuite("SimpleTest");

    CppUnit_addTest(pSuite, SimpleTest, testString);
    CppUnit_addTest(pSuite, SimpleTest, testNumber);
    //pSuite->addTest(new CppUnit::TestCaller<SimpleTest>("SimpleTest", &SimpleTest::testString));
    //pSuite->addTest(new CppUnit::TestCaller<SimpleTest>("SimpleTest", &SimpleTest::testNumber));

    return pSuite;
}