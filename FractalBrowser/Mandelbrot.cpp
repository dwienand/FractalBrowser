//
//  Mandelbrot.cpp
//  FractalBrowser
//
//  MandelBrot class that can calculate itself to an array
//
//  Created by Dominik on 3/30/15.
//  Copyright (c) 2015 Team060. All rights reserved.
//

#include "Mandelbrot.h"
using std::cout;
using std::endl;
using std::vector;

vector<unsigned int>* ReadBMP(char* filename)
{
    FILE* f = fopen(filename, "rb");
    
    if(f == NULL)
        throw "File not found!";
    
    unsigned char info[54];
    fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header
    
    // extract image height and width from header
    int width = *(int*)&info[18];
    int height = *(int*)&info[22];
    
    cout << endl;
    cout << "  Name: " << filename << endl;
    cout << " Width: " << width << endl;
    cout << "Height: " << height << endl;
    
    int row_padded = (width*3 + 3) & (~3);
    unsigned char* data = new unsigned char[row_padded];
    
    fread(data, sizeof(unsigned char), row_padded, f);
    
    vector<unsigned int>* palette = new vector<unsigned int>(0);
    for(int j = 0; j < width*3; j += 3)
    {
        unsigned int color = data[j] | (data[j+1] << 8) | (data[j+2] << 16);
        palette->push_back(color);
    }
    
    delete data;
    
    fclose(f);
    return palette;
}

Mandelbrot::Mandelbrot(int width, int height){
    this->width = width;
    this->height = height;
    
    mandelbrotInt = new unsigned int[width * height];
    memset(mandelbrotInt, 0, width * height * sizeof(unsigned int));
    
    mandelbrotFloat = new double[width * height];
    memset(mandelbrotFloat, 0.0, width * height * sizeof(double));
    
    mandelbrotPixels = new unsigned int[width*height];
    memset(mandelbrotPixels, 0, width*height*sizeof(unsigned int));
    
    
    char* filename ="./color_palettes/test_gradient.bmp";
    palette = ReadBMP(filename);
    
    
    for (int i = 0; i < palette->size(); i++){
        cout << (*palette)[i];
    }
}

Mandelbrot::~Mandelbrot(){
    delete this->mandelbrotInt;
    delete this->mandelbrotFloat;
}



void Mandelbrot::render(){
    LOG(INFO) << "Rendering Mandelbrot with coordinates real left: " << leftReal << ", real right: " << rightReal << ", imaginary lower: " << lowerImag << ", imaginary upper: " << upperImag << "\n" ;
    
    long startTime = std::chrono::duration_cast< std::chrono::milliseconds >(
                                                                             std::chrono::high_resolution_clock::now().time_since_epoch()
                                                                             ).count();
    memset(mandelbrotInt, 0, width * height * sizeof(unsigned int));
    
    
    for(int px = 0; px < width; px++){
        for(int py = 0; py < height; py++){
            int iterations = 0;
            double x = 0.0;
            double y = 0.0;
            //convert pixel x coordinate to coordinate on real axis of complex plane
            double x0 = leftReal + ((double) px) / ((double) width) * (rightReal-leftReal);
            //convert pixel y coordinate to coordinate on imaginary axis of complex plane
            double y0 = lowerImag + ((double)py) / ((double) height) * (upperImag - lowerImag);
            
            //do iteration step
            while(x*x + y*y < escapeRadius && iterations < maxIterations){
                double xtemp = x*x - y*y + x0;
                y = 2*x*y + y0;
                x = xtemp;
                iterations++;
            }
            
            //save raw iteration count
            mandelbrotInt[py*width+px] = iterations;
            
            
            
            if (iterations < maxIterations){
                double sqrxy = x * x + y * y;
                float modulus = sqrt (x*x + y*y);
                float mu = iterations - (log (log (modulus)))/ log (2.0);
                mandelbrotFloat[py*width+px] = mu / maxIterations;
            }
            else
                mandelbrotFloat[py*width+px] = 0;
            
        }
        
        
    }
    applyColorFilter();
    
    /*
    for(int i = 0; i < width; i++){
        for(int j = 0; j < height; j++){
            cout << mandelbrotFloat[width*j + i] << ";";
        }
        cout << "\n";
    }
    
     */
    long endTime = std::chrono::duration_cast< std::chrono::milliseconds >(
                                                                           std::chrono::high_resolution_clock::now().time_since_epoch()
                                                                           ).count();
    long runTime = endTime - startTime;
    LOG(INFO) << "Finished rendering Mandelbrot, took " << runTime << "ms.";
    
}

void Mandelbrot::applyColorFilter(){
    for(int i = 0; i < width; i++)
        for(int j = 0; j < height; j++){
            mandelbrotPixels[j*width+i] = (this->*colorFilters[colorFilterIndex])(mandelbrotInt[j*width+i], mandelbrotFloat[j*width+i]);
        }
    
}

//
// Color filters
//

inline unsigned int Mandelbrot::bwFilter(unsigned int iterations, double floatPart){
    if (iterations >= maxIterations){
        return WHITE;
    }
    else{
        return 0;
    }
}

inline unsigned int Mandelbrot::redFilter(unsigned int iterations, double floatPart){
    if (iterations >= maxIterations){
        return RED;
    }
    else{
        double relativeBrightness = (double) iterations / (double) maxIterations;
        unsigned int redTone =  (unsigned int) (((double) RED)*relativeBrightness);
        return redTone;
    }
}

inline unsigned int Mandelbrot::greenFilter(unsigned int iterations, double floatPart){
    if (iterations >= maxIterations){
        return GREEN;
    }
    else{
        double relativeBrightness = (double) iterations / (double) maxIterations;
        unsigned int greenTone =  (unsigned int) (((double) GREEN)*relativeBrightness);
        return greenTone;
    }
}

inline unsigned int Mandelbrot::blueFilter(unsigned int iterations, double floatPart){
    if (iterations >= maxIterations){
        return BLUE;
    }
    else{
        double relativeBrightness = (double) iterations / (double) maxIterations;
        unsigned int blueTone =  (unsigned int) (((double) BLUE)*relativeBrightness);
        return blueTone;
    }
}

unsigned int Mandelbrot::paletteFilter(unsigned int iterations, double floatPart){
    if (iterations >= maxIterations){
        return 0;
    }
    int idx = iterations % palette->size();
    unsigned int color = (*palette)[idx];
    return color;

}

inline unsigned int Mandelbrot::continuousColoring(unsigned int iterations, double floatPart){
    int* palette = new int[256];
    for (int i=0; i < 256; i++) {
        palette[i] = i;
    }
    
    if (iterations < maxIterations){
        int colorIndex = iterations % 256;
        unsigned int color1 = palette[colorIndex];
        unsigned int color2 = palette[colorIndex + 1];
        // iteration % 1 = fractional part of iteration.
        unsigned int color = (unsigned int) (color2 - color1) * floatPart;
        
        return color;
    }
    return 0;
    
}

void Mandelbrot::rotateColorFilterLeft(){
    if (colorFilterIndex == 0)
        colorFilterIndex = colorFilterCount - 1;
    else
        colorFilterIndex--;
    LOG(INFO) << "Switched color filter to #" << colorFilterIndex;
}

void Mandelbrot::rotateColorFilterRight(){
    colorFilterIndex = (colorFilterIndex + 1) % colorFilterCount;
    
    LOG(INFO) << "Switched color filter to #" << colorFilterIndex;
}


//
// Navigating the Mandelbrot
//

void Mandelbrot::moveFrameUp(){
    double frameHeight = upperImag - lowerImag;
    double stepSize = frameHeight * moveSpeed;
    upperImag -= stepSize;
    lowerImag -= stepSize;
    
}

void Mandelbrot::moveFrameDown(){
    double frameHeight = upperImag - lowerImag;
    double stepSize = frameHeight * moveSpeed;
    upperImag += stepSize;
    lowerImag += stepSize;
    
}

void Mandelbrot::moveFrameLeft(){
    double frameWidth = rightReal - leftReal;
    double stepSize = frameWidth * moveSpeed;
    
    leftReal -= stepSize;
    rightReal -= stepSize;
    
}

void Mandelbrot::moveFrameRight(){
    double frameWidth = rightReal - leftReal;
    double stepSize = frameWidth * moveSpeed;
    
    leftReal += stepSize;
    rightReal += stepSize;
    
}

void Mandelbrot::zoomIn(){
    double frameHeight = upperImag - lowerImag;
    double frameWidth = rightReal - leftReal;
    
    double verticalStepSize = frameHeight * zoomSpeed;
    double horizontalStepSize = frameWidth * zoomSpeed;
    
    leftReal += horizontalStepSize;
    rightReal -= horizontalStepSize;
    
    upperImag -= verticalStepSize;
    lowerImag += verticalStepSize;
}


void Mandelbrot::zoomOut(){
    
    double frameHeight = upperImag - lowerImag;
    double frameWidth = rightReal - leftReal;
    
    double verticalStepSize = frameHeight * zoomSpeed;
    double horizontalStepSize = frameWidth * zoomSpeed;
    
    leftReal -= horizontalStepSize;
    rightReal += horizontalStepSize;
    
    upperImag += verticalStepSize;
    lowerImag -= verticalStepSize;
    
}

void Mandelbrot::reset(){
    leftReal = initialLeftReal;
    rightReal = initialRightReal;
    lowerImag = initialLowerImag;
    upperImag = initialUpperImag;
}





