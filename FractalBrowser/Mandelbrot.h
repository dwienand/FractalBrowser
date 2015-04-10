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
#include <cmath>
#include "easylogging++.h"


class Mandelbrot {
private:
    int width, height;
    unsigned int* mandelbrotInt;
    double* mandelbrotFloat;
    unsigned int* mandelbrotPixels;
    
    unsigned int maxIterations = 1000;
    
    //color constants
    const unsigned int RED = 255 << 16;
    const unsigned int GREEN = 255 << 8;
    const unsigned int BLUE = 255;
    const unsigned int WHITE = RED | GREEN | BLUE;
    
    //Mandelbrot dimensions
    const double initialLeftReal = -2.5;
    const double initialRightReal = 1.0;
    const double initialLowerImag = -1.0;
    const double initialUpperImag = 1.0;
    
    double leftReal = initialLeftReal;
    double rightReal = initialRightReal;
    double lowerImag = initialLowerImag;
    double upperImag = initialUpperImag;
    
    
    double moveSpeed = 0.1;
    double zoomSpeed = 0.1;
    
    // variables for managing color filters
    // to include a new filter, add it to the colorFilters array and increment colorFilterCount accordingly
    unsigned int colorFilterIndex = 1;
    const static unsigned int colorFilterCount = 4;
    unsigned int mapColor(unsigned int iterations);
    
    unsigned int bwFilter(unsigned int iterations, double floatPart);
    unsigned int redFilter(unsigned int iterations, double floatPart);
    unsigned int greenFilter(unsigned int iterations, double floatPart);
    unsigned int blueFilter(unsigned int iterations, double floatPart);
    unsigned int continuousColoring(unsigned int iterations, double floatPart);
    
    typedef unsigned int(Mandelbrot::*FunctionPointer)(unsigned int, double floatPart);
    FunctionPointer colorFilters[colorFilterCount] = {
        &Mandelbrot::bwFilter,
        &Mandelbrot::redFilter,
        &Mandelbrot::greenFilter,
        &Mandelbrot::blueFilter
    };
    
    
public:
    Mandelbrot(int width, int height);
    ~Mandelbrot();
    
    void render();
    void applyColorFilter();
    
    void moveFrameUp();
    void moveFrameDown();
    void moveFrameLeft();
    void moveFrameRight();
    void zoomIn();
    void zoomOut();
    void reset();
    
    void rotateColorFilterLeft();
    void rotateColorFilterRight();
    
    unsigned int* getMandelbrotPixels(){return this->mandelbrotPixels;};
    
};

#endif /* defined(__FractalBrowser__Mandelbrot__) */
