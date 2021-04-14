#include "SimpleTest.h"
#include "cppunit/ui/text/TestRunner.h"

int main()
{
    CppUnit::TextUi::TestRunner runner;

    runner.addTest(SimpleTest::suite()); //指定运行TestSuite

    //开始运行, 自动显示测试进度和测试结果

    runner.run("", true);

    return 0;
}