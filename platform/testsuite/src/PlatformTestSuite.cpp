//
// Created by Ivan Kishchenko on 2019-06-16.
//

#include "JsonBaseTest.h"
#include <cppunit/TextTestRunner.h>
#include <cppunit/TestSuite.h>

#include <cppunit/extensions/HelperMacros.h>


int main(int ac, char **av) {
    CppUnit::Test *test =
            CppUnit::TestFactoryRegistry::getRegistry().makeTest();
    CppUnit::TextTestRunner runner;
    runner.addTest(test);

    runner.run();
    return 0;
}