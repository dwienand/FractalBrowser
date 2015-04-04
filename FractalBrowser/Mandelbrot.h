//
//  Mandelbrot.h
//  FractalBrowser
//
//  Created by Dominik on 3/30/15.
//  Copyright (c) 2015 Team060. All rights reserved.
//

#ifndef __FractalBrowser__Mandelbrot__
#define __FractalBrowser__Mandelbrot__

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <chrono>
#include "easylogging++.h"


class Mandelbrot {
private:
    int width, height;
    unsigned int* pixels;
    
    unsigned int maxIterations = 1000;
    
    const unsigned int RED = 255 << 16;
    const unsigned int GREEN = 255 << 8;
    const unsigned int BLUE = 255;
    const unsigned int WHITE = RED | GREEN | BLUE;
    
    double leftReal = -2.5;
    double rightReal = 1.0;
    double lowerImag = -1.0;
    double upperImag = 1.0;
    
    double moveSpeed = 0.1;
    
    double zoomSpeed = 0.1;
    
    
    unsigned int colorFilterIndex = 1;
    const static unsigned int colorFilterCount = 4;
    unsigned int mapColor(unsigned int iterations);
    
    unsigned int bwFilter(unsigned int iterations);
    unsigned int redFilter(unsigned int iterations);
    unsigned int greenFilter(unsigned int iterations);
    unsigned int blueFilter(unsigned int iterations);
    
    
    
    typedef unsigned int(Mandelbrot::*FunctionPointer)(unsigned int);
    FunctionPointer colorFilters[colorFilterCount] = {
        &Mandelbrot::bwFilter,
        &Mandelbrot::redFilter,
        &Mandelbrot::greenFilter,
        &Mandelbrot::blueFilter
        };

    
public:
    Mandelbrot(int width, int height);
    
    unsigned int* render();
    
    void moveFrameUp();
    void moveFrameDown();
    void moveFrameLeft();
    void moveFrameRight();
    void zoomIn();
    void zoomOut();
    
    void rotateColorFilterLeft();
    void rotateColorFilterRight();
    
};

#endif /* defined(__FractalBrowser__Mandelbrot__) */
