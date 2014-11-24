#ifndef TEST2_H
#define TEST2_H

#include "AutoTest.h"

class Test2 : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void test1();
    void test2();
    void cleanupTestCase();
};

DECLARE_TEST(Test2)

#endif // TEST2_H
