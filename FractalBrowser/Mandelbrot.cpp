//
//  Mandelbrot.cpp
//  FractalBrowser
//
//  Created by Dominik on 3/30/15.
//  Copyright (c) 2015 Team060. All rights reserved.
//

#include "Mandelbrot.h"
using std::cout;

Mandelbrot::Mandelbrot(int width, int height){
    this->width = width;
    this->height = height;
    
    pixels = new unsigned int[width * height];
    memset(pixels, 0, width * height * sizeof(unsigned int));
}



unsigned int* Mandelbrot::render(){    
        
    memset(pixels, 0, width * height * sizeof(unsigned int));

    
    for(int px = 0; px < width; px++){
        for(int py = 0; py < height; py++){
            int iteration = 0;
            double x = 0.0;
            double y = 0.0;
            double x0 = leftReal + ((double) px) / ((double) width) * (rightReal-leftReal);
            double y0 = lowerImag + ((double)py) / ((double) height) * (upperImag - lowerImag);
            
            while(x*x + y*y < 4 && iteration < maxIterations){
                double xtemp = x*x - y*y + x0;
                y = 2*x*y + y0;
                x = xtemp;
                iteration++;
            }
            pixels[py*width+px] = mapColor(iteration);
            
        }
    }
    
    return pixels;
    
}

inline unsigned int Mandelbrot::mapColor(unsigned int iterations){
    if (iterations >= maxIterations){
        return 255;
    }
    else{
        return 0;
    }
}


