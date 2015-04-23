//
//  main.cpp
//  UnitTests
//
//  Created by Grivan Thapar on 4/10/15.
//  Copyright (c) 2015 Team060. All rights reserved.
//

#include <stdio.h>

#include "gtest/gtest.h"
#include "easylogging++.h"

INITIALIZE_EASYLOGGINGPP



GTEST_API_ int main(int argc, char **argv) {
    el::Configurations conf("./logging.conf");
    el::Loggers::reconfigureAllLoggers(conf);
    printf("Running main() from gtest_main.cc\n");
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
