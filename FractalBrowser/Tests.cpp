//
//  Tests.cpp
//  FractalBrowser
//
//  Created by Grivan Thapar on 4/10/15.
//  Copyright (c) 2015 Team060. All rights reserved.
//

#include "Tests.h"
#include "gtest/gtest.h"
#include "Mandelbrot.h"

// Tests factorial of negative numbers.
TEST(FractalTest, RenderSpeed) {
    // This test is named "Negative", and belongs to the "FactorialTest"
    // test case.
    
    int height = 500;
    int width = 500;
    Mandelbrot* set = new Mandelbrot(height,width);
    
    set->render();
}