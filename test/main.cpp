#include "AutoTest.h"
#include <QDebug>

#if 1
// default runner
TEST_MAIN
#else
// supply own main function
int main(int argc, char *argv[])
{
    int failures = AutoTest::run(argc, argv);
    if (failures == 0) {
        qDebug() << "ALL TESTS PASSED";
    } else {
        qDebug() << failures << " TESTS FAILED!";
    }
    return failures;
}
#endif
