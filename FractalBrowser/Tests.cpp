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


#define WIDTH 1000
#define HEIGHT 1000


// Tests factorial of negative numbers.
TEST(FractalTest, RenderSpeed) {
    // This test is named "Negative", and belongs to the "FactorialTest"
    // test case.
    
    Mandelbrot* set = new Mandelbrot(WIDTH,HEIGHT);
    
    set->render();
}

void tourMandelbrot(Mandelbrot* mandelbrot){
    mandelbrot->render();
    
    mandelbrot->moveFrameRight();
    mandelbrot->render();
    mandelbrot->moveFrameDown();
    mandelbrot->render();
    mandelbrot->moveFrameLeft();
    mandelbrot->render();
    mandelbrot->moveFrameUp();
    
    mandelbrot->zoomIn();
    mandelbrot->render();
    mandelbrot->zoomIn();
    mandelbrot->render();
    
    mandelbrot->moveFrameRight();
    mandelbrot->render();
    mandelbrot->moveFrameDown();
    mandelbrot->render();
    mandelbrot->moveFrameLeft();
    mandelbrot->render();
    mandelbrot->moveFrameUp();
}

void setBaseline(Mandelbrot* mandelbrot){
    mandelbrot->checkSymmetry = false ;
    mandelbrot->checkCardioidAndBulb = false ;
    mandelbrot->moveWithSlices = false ;
    mandelbrot->checkPeriodicity = false;
}

void setAllOptimizationsOn(Mandelbrot* mandelbrot){
    mandelbrot->checkSymmetry = true ;
    mandelbrot->checkCardioidAndBulb = true ;
    mandelbrot->moveWithSlices = true ;
    mandelbrot->checkPeriodicity = true;
}

TEST(FractalTest, MandelbrotTourBaseLine){
    Mandelbrot* mandelbrot = new Mandelbrot(WIDTH, HEIGHT);
    setBaseline( mandelbrot);
    tourMandelbrot(mandelbrot);
}

TEST(FractalTest, MandelbrotTourAllOptimizations){
    Mandelbrot* mandelbrot = new Mandelbrot(WIDTH, HEIGHT);
    setAllOptimizationsOn(mandelbrot);
    
    tourMandelbrot(mandelbrot);
}



TEST(FractalTest, MandelbrotTourSymmetry){
    Mandelbrot* mandelbrot = new Mandelbrot(WIDTH, HEIGHT);
    setBaseline( mandelbrot);
    mandelbrot->checkSymmetry = true ;
    
    tourMandelbrot(mandelbrot);
}

TEST(FractalTest, MandelbrotTourCardioidAndBulb){
    Mandelbrot* mandelbrot = new Mandelbrot(WIDTH, HEIGHT);
    setBaseline( mandelbrot);
    
    mandelbrot->checkCardioidAndBulb = true ;
    
    tourMandelbrot(mandelbrot);
}

TEST(FractalTest, MandelbrotTourSlices){
    Mandelbrot* mandelbrot = new Mandelbrot(WIDTH, HEIGHT);
    setBaseline( mandelbrot);
    
    mandelbrot->moveWithSlices = true ;
    
    tourMandelbrot(mandelbrot);
}


TEST(FractalTest, MandelbrotTourPeriodicity){
    Mandelbrot* mandelbrot = new Mandelbrot(WIDTH, HEIGHT);
    setBaseline( mandelbrot);
    
    mandelbrot->checkPeriodicity = true;
    
    tourMandelbrot(mandelbrot);
}



TEST(FractalTest, MandelbrotTourBaseline1Thread){
    Mandelbrot* mandelbrot = new Mandelbrot(WIDTH, HEIGHT);
    setBaseline( mandelbrot);
    mandelbrot->numThreads = 1;
    
    tourMandelbrot(mandelbrot);
}

TEST(FractalTest, MandelbrotTourBaseline2Threads){
    Mandelbrot* mandelbrot = new Mandelbrot(WIDTH, HEIGHT);
    setBaseline( mandelbrot);
    mandelbrot->numThreads = 2;
    
    tourMandelbrot(mandelbrot);
}

TEST(FractalTest, MandelbrotTourBaseline3Threads){
    Mandelbrot* mandelbrot = new Mandelbrot(WIDTH, HEIGHT);
    setBaseline( mandelbrot);
    mandelbrot->numThreads = 3;
    
    tourMandelbrot(mandelbrot);
}

TEST(FractalTest, MandelbrotTourBaseline4Threads){
    Mandelbrot* mandelbrot = new Mandelbrot(WIDTH, HEIGHT);
    setBaseline( mandelbrot);
    mandelbrot->numThreads = 4;
    
    tourMandelbrot(mandelbrot);
}

TEST(FractalTest, MandelbrotTourBaseline5Threads){
    Mandelbrot* mandelbrot = new Mandelbrot(WIDTH, HEIGHT);
    setBaseline( mandelbrot);
    mandelbrot->numThreads = 5;
    
    tourMandelbrot(mandelbrot);
}

TEST(FractalTest, MandelbrotTourBaseline6Threads){
    Mandelbrot* mandelbrot = new Mandelbrot(WIDTH, HEIGHT);
    setBaseline( mandelbrot);
    mandelbrot->numThreads = 6;
    
    tourMandelbrot(mandelbrot);
}

TEST(FractalTest, MandelbrotTourBaseline7Threads){
    Mandelbrot* mandelbrot = new Mandelbrot(WIDTH, HEIGHT);
    setBaseline( mandelbrot);
    mandelbrot->numThreads = 7;
    
    tourMandelbrot(mandelbrot);
}

TEST(FractalTest, MandelbrotTourBaseline8Threads){
    Mandelbrot* mandelbrot = new Mandelbrot(WIDTH, HEIGHT);
    setBaseline( mandelbrot);
    mandelbrot->numThreads = 8;
    
    tourMandelbrot(mandelbrot);
}




TEST(FractalTest, MandelbrotTourAllOptimizations1Threads){
    Mandelbrot* mandelbrot = new Mandelbrot(WIDTH, HEIGHT);
    setAllOptimizationsOn( mandelbrot);
    mandelbrot->numThreads = 1;
    
    tourMandelbrot(mandelbrot);
}

TEST(FractalTest, MandelbrotTourAllOptimizations2Threads){
    Mandelbrot* mandelbrot = new Mandelbrot(WIDTH, HEIGHT);
    setAllOptimizationsOn( mandelbrot);
    mandelbrot->numThreads e= 2;
    
    tourMandelbrot(mandelbrot);
}

TEST(FractalTest, MandelbrotTourAllOptimizations3Threads){
    Mandelbrot* mandelbrot = new Mandelbrot(WIDTH, HEIGHT);
    setAllOptimizationsOn( mandelbrot);
    mandelbrot->numThreads = 3;
    
    tourMandelbrot(mandelbrot);
}

TEST(FractalTest, MandelbrotTourAllOptimizations4Threads){
    Mandelbrot* mandelbrot = new Mandelbrot(WIDTH, HEIGHT);
    setAllOptimizationsOn( mandelbrot);
    mandelbrot->numThreads = 4;
    
    tourMandelbrot(mandelbrot);
}

TEST(FractalTest, MandelbrotTourAllOptimizations5Threads){
    Mandelbrot* mandelbrot = new Mandelbrot(WIDTH, HEIGHT);
    setAllOptimizationsOn( mandelbrot);
    mandelbrot->numThreads = 5;
    
    tourMandelbrot(mandelbrot);
}

TEST(FractalTest, MandelbrotTOurAllOptimizations6Threads){
    Mandelbrot* mandelbrot = new Mandelbrot(WIDTH, HEIGHT);
    setAllOptimizationsOn( mandelbrot);
    mandelbrot->numThreads = 6;
    
    tourMandelbrot(mandelbrot);
}

TEST(FractalTest, MandelbrotTourAllOptimizations7Threads){
    Mandelbrot* mandelbrot = new Mandelbrot(WIDTH, HEIGHT);
    setAllOptimizationsOn( mandelbrot);
    mandelbrot->numThreads = 7;
    
    tourMandelbrot(mandelbrot);
}

TEST(FractalTest, MandelbrotTourAllOptimizations8Threads){
    Mandelbrot* mandelbrot = new Mandelbrot(WIDTH, HEIGHT);
    setAllOptimizationsOn( mandelbrot);
    mandelbrot->numThreads = 8;
    
    tourMandelbrot(mandelbrot);
}

TEST(FractalTest, RotateColorPalette){
    Mandelbrot* mandelbrot = new Mandelbrot(WIDTH, HEIGHT);
    for(int i = 0; i < 10; i++)
        mandelbrot->rotateColorFilterRight();
}



