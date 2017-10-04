#include "unit_test_datatypes.h"

void unit_test_datatypes::testIsParsableIsType(){
    QString str = "Hello";
    QCOMPARE(str.toUpper(), QString("HELLO"));
}

QTEST_MAIN(unit_test_datatypes)
