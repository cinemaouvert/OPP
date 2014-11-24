#ifndef TEST1_H
#define TEST1_H

#include "AutoTest.h"

class Test1 : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void test1();
    void test2();
    void cleanupTestCase();
};

DECLARE_TEST(Test1)

#endif // TEST1_H
