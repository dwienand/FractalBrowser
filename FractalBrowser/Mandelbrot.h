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


class Mandelbrot {
private:
    int width, height;
    unsigned int* pixels;
    
    unsigned int maxIterations = 1000;
    
    double leftReal = -2.5;
    double rightReal = 1.0;
    double lowerImag = -1.0;
    double upperImag = 1.0;
    
    unsigned int mapColor(unsigned int iterations);

    
public:
    Mandelbrot(int width, int height);
    
    unsigned int* render();
    
};

#endif /* defined(__FractalBrowser__Mandelbrot__) */
