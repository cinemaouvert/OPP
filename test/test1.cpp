#include "test1.h"

void Test1::initTestCase()
{
}

void Test1::test1()
{
    QVERIFY(1 + 1 == 2);
}

void Test1::test2()
{
    QVERIFY(1 == 0);
}

void Test1::cleanupTestCase()
{
}
