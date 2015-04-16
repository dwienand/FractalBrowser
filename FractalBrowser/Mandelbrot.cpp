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
using std::vector;



Mandelbrot::Mandelbrot(int width, int height){
    this->width = width;
    this->height = height;
    
    mandelbrotInt = new unsigned int[width * height];
    memset(mandelbrotInt, 0, width * height * sizeof(unsigned int));
    
    mandelbrotFloat = new double[width * height];
    memset(mandelbrotFloat, 0.0, width * height * sizeof(double));
    
    mandelbrotPixels = new unsigned int[width*height];
    memset(mandelbrotPixels, 0, width*height*sizeof(unsigned int));
    
    
    const char* filename ="./color_palettes/test_gradient.bmp";
    palette = ReadBMP(filename);
    
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
            double ZReal = 0.0;
            double ZImag = 0.0;
            //convert pixel x coordinate to coordinate on real axis of complex plane
            double CReal = leftReal + ((double) px) / ((double) width) * (rightReal-leftReal);
            //convert pixel y coordinate to coordinate on imaginary axis of complex plane
            double CImag = lowerImag + ((double)py) / ((double) height) * (upperImag - lowerImag);
            
            //do iteration step
            while(ZReal*ZReal + ZImag*ZImag < escapeRadius && iterations < maxIterations){
                double ZRealTemp = ZReal*ZReal - ZImag*ZImag + CReal;
                ZImag = 2*ZReal*ZImag + CImag;
                ZReal = ZRealTemp;
                iterations++;
            }
            
            //save raw iteration count
            mandelbrotInt[py*width+px] = iterations;
            
            
            
            if (iterations < maxIterations){
                
                double modulus = sqrt (ZReal * ZReal + ZImag * ZImag);
                double nu = log(log(modulus)/log(2))/log(2);
                mandelbrotFloat[py*width+px] = iterations + 1 - nu;
            }
            else
                mandelbrotFloat[py*width+px] = maxIterations;
            
        }
        
        
    }
    applyColorFilter();
    
    
    for(int i = 0; i < width; i++){
        for(int j = 0; j < height; j++){
            //printf("%.2f;", mandelbrotInt[width*j + i]);
            //cout <<mandelbrotInt[width*j + i] << ";";
        }
        //cout << "\n";
    }
    
     
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
        return 0xffff00;
    }
    double relativePos = log(iterations)/ log(maxIterations);
    int idx = ( (int) ( (double) palette->size()) * relativePos);
    unsigned int color = (*palette)[idx];
    return color;

}

inline unsigned int Mandelbrot::continuousColoring(unsigned int iterations, double floatPart){
    
    if (floatPart >= maxIterations){
        return 0xffff00;
    }
    double relativePos = log(floatPart)/ log(maxIterations);
    int idx = ( (int) ( (double) palette->size()) * relativePos);
    unsigned int color = (*palette)[idx];
    return color;

    
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


void Mandelbrot::setDimensions(double leftReal, double rightReal, double lowerImag, double upperImag){
    this->leftReal = leftReal;
    this->rightReal = rightReal;
    this->lowerImag = lowerImag;
    this->upperImag = upperImag;
}

void Mandelbrot::doubleIterations(){
    if(this->maxIterations * 2 > this->maxIterations)
        this->maxIterations *= 2;
    LOG(INFO) << "Increased number of iteration to " << this->maxIterations << "\n";
}

void Mandelbrot::halfIterations(){
    this->maxIterations /= 2;
    if (this->maxIterations == 0)
        this->maxIterations = 1;
    LOG(INFO) << "Decreased number of iteration to " << this->maxIterations << "\n";
    
}





