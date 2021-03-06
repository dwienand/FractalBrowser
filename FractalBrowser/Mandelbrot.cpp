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
    
    
    mandelbrotIntTemp = new unsigned int[width * height];
    memset(mandelbrotIntTemp, 0, width * height * sizeof(unsigned int));
    
    mandelbrotFloatTemp = new double[width * height];
    memset(mandelbrotFloatTemp, 0.0, width * height * sizeof(double));
    
    mandelbrotPixelsTemp = new unsigned int[width*height];
    memset(mandelbrotPixelsTemp, 0, width*height*sizeof(unsigned int));
    
    
    const char* filename ="./color_palettes/test_gradient.bmp";
    palette = ReadBMP(filename);
    
}

Mandelbrot::~Mandelbrot(){
    delete this->mandelbrotInt;
    delete this->mandelbrotFloat;
    delete this->mandelbrotPixels;
    
    delete this->mandelbrotIntTemp;
    delete this->mandelbrotFloatTemp;
    delete this->mandelbrotPixelsTemp;
}


void Mandelbrot::calculateMandelbrotMultithreaded(){
    vector<std::thread> t(numThreads);
    
    //Launch a group of threads
    for (int i = 0; i < numThreads; ++i) {
        t[i] = std::thread(&Mandelbrot::calculateMandelbrotColThread, this, i);
    }
    
    //Join the threads with the main thread
    for (int i = 0; i < numThreads; ++i) {
        t[i].join();
    }
    
}

void* Mandelbrot::calculateMandelbrotColThread(int threadID){
    for(int px = threadID; px < width; px+=numThreads)
        for(int py = 0; py < height; py++){
            calculateMandelbrotPoint(px, py);
            
        }
    
    return NULL;
    
}


inline void Mandelbrot::calculateMandelbrotPoint(int px, int py){
    
    
    int iterations = 0;
    double ZReal = 0.0;
    double ZImag = 0.0;
    //convert pixel x coordinate to coordinate on real axis of complex plane
    double CReal = leftReal + ((double) px) / ((double) width) * (rightReal-leftReal);
    //convert pixel y coordinate to coordinate on imaginary axis of complex plane
    double CImag = upperImag - ((double)py) / ((double) height) * (upperImag - lowerImag);
    
    
    //we know the fractal has a main cardioid and a period 2 bulb
    // that we can just set to max iteration right away
    if(checkCardioidAndBulb)
        if (cardioidTest(CReal, CImag) || period2BulbTest(CReal, CImag)){
            mandelbrotInt[py*width+px] = maxIterations;
            mandelbrotFloat[py*width+px] = maxIterations;
            return;
        }
    
    // check symmetry
    if(checkSymmetry)
        if (CImag < 0 && -CImag < upperImag){
            //assume we're rendering from bottom to top
            //find mirrored py value
            int mirroredPy = mirrorPy(py);
            //check if mirrored pixel has actually been assigned, prevents black line from appearing at middle of mandelbrot
            if (mandelbrotInt[mirroredPy*width+px] != 0){
                mandelbrotInt[py*width+px] = mandelbrotInt[mirroredPy*width+px];
                mandelbrotFloat[py*width+px] = mandelbrotFloat[mirroredPy*width+px];
                
                return;
            }
        }
    
    
    //do iteration step
    while(ZReal*ZReal + ZImag*ZImag < escapeRadius && iterations < maxIterations){
        double ZRealTemp = ZReal*ZReal - ZImag*ZImag + CReal;
        double ZImagTemp = 2*ZReal*ZImag + CImag;
        
        //check for periodicity
        if(checkPeriodicity)
            if (ZRealTemp == ZReal  &&  ZImagTemp == ZImag)
            {
                mandelbrotInt[py*width+px] = maxIterations;
                mandelbrotFloat[py*width+px] = maxIterations;
                return;
            }
        
        ZReal = ZRealTemp;
        ZImag = ZImagTemp;
        iterations++;
    }
    
    //save raw iteration count
    mandelbrotInt[py*width+px] = iterations;
    
    
    
    if (iterations < maxIterations){
        
        double modulus = sqrt (ZReal * ZReal + ZImag * ZImag);
        double nu = log(log(modulus)/log(2))/log(2);
        
        double floatVal = iterations + 1 - nu;
        mandelbrotFloat[py*width+px] =floatVal;
        
        if (floatVal != floatVal){
            cout << " nu: " << nu << "\n";
        }
        
        
    }
    else
        mandelbrotFloat[py*width+px] = maxIterations;
    
}



void Mandelbrot::render(){
    LOG(INFO) << "Rendering Mandelbrot with coordinates real left: " << leftReal << ", real right: " << rightReal << ", imaginary lower: " << lowerImag << ", imaginary upper: " << upperImag << "\n" ;
    
    long startTime = std::chrono::duration_cast< std::chrono::milliseconds >(
                                                                             std::chrono::high_resolution_clock::now().time_since_epoch()
                                                                             ).count();
    
    if(canReuseFrame){
        canReuseFrame = false;
    }
    else{
        calculateMandelbrotMultithreaded();
    }
    applyColorFilter();
    
    
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


inline unsigned int Mandelbrot::cyclicColoring(unsigned int iterations, double floatPart){
    
    if (floatPart >= maxIterations){
        return 0x0;
    }
    int idx = (int) round(floatPart);
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
    
    
    if(moveWithSlices){
        int pixelSteps = moveSpeed * height;
        
        Mandelbrot* newSlice = new Mandelbrot(width, pixelSteps);
        newSlice->setDimensions(leftReal, rightReal, upperImag, upperImag + stepSize);
        newSlice->render();
        
        for(int i = 0; i < width; i++){
            for (int j = 0; j < height; j++) {
                if(j >= pixelSteps){
                    mandelbrotFloatTemp[j*width + i] = mandelbrotFloat[(j-pixelSteps) * width + i ];
                    mandelbrotIntTemp[j*width + i] = mandelbrotInt[(j-pixelSteps) * width + i ];
                    mandelbrotPixelsTemp[j*width + i] = mandelbrotPixels[(j-pixelSteps) * width + i ];
                }
                else{
                    mandelbrotFloatTemp[j*width + i] =newSlice->mandelbrotFloat[j * newSlice->width + i ];
                    mandelbrotIntTemp[j*width + i] = newSlice->mandelbrotInt[j * newSlice->width + i ];
                    mandelbrotPixelsTemp[j*width + i] = newSlice->mandelbrotPixels[j * newSlice->width + i ];
                }
            }
        }
        
        std::memcpy(mandelbrotFloat, mandelbrotFloatTemp, width*height*sizeof( double));
        std::memcpy(mandelbrotInt, mandelbrotIntTemp, width*height*sizeof( unsigned int));
        std::memcpy(mandelbrotPixels, mandelbrotPixelsTemp, width*height*sizeof( unsigned int));
        
        delete newSlice;
        
        canReuseFrame = true;
    }
    
    upperImag += stepSize;
    lowerImag += stepSize;
    
}

void Mandelbrot::moveFrameDown(){
    double frameHeight = upperImag - lowerImag;
    double stepSize = frameHeight * moveSpeed;
    
    
    if(moveWithSlices){
        int pixelSteps = moveSpeed * height;
        
        Mandelbrot* newSlice = new Mandelbrot(width, pixelSteps);
        newSlice->setDimensions(leftReal, rightReal, lowerImag - stepSize, lowerImag );
        newSlice->render();
        
        for(int i = 0; i < width; i++){
            for (int j = 0; j < height; j++) {
                if(j < height - pixelSteps){
                    mandelbrotFloatTemp[j*width + i] = mandelbrotFloat[(j + pixelSteps) * width + i ];
                    mandelbrotIntTemp[j*width + i] = mandelbrotInt[(j + pixelSteps) * width + i ];
                    mandelbrotPixelsTemp[j*width + i] = mandelbrotPixels[(j +pixelSteps) * width + i ];
                }
                else{
                    mandelbrotFloatTemp[j*width + i] =newSlice->mandelbrotFloat[(j - (height-pixelSteps)) * newSlice->width + i ];
                    mandelbrotIntTemp[j*width + i] = newSlice->mandelbrotInt[(j - (height-pixelSteps)) * newSlice->width + i ];
                    mandelbrotPixelsTemp[j*width + i] = newSlice->mandelbrotPixels[(j - (height-pixelSteps)) * newSlice->width + i ];
                }
            }
        }
        
        std::memcpy(mandelbrotFloat, mandelbrotFloatTemp, width*height*sizeof( double));
        std::memcpy(mandelbrotInt, mandelbrotIntTemp, width*height*sizeof( unsigned int));
        std::memcpy(mandelbrotPixels, mandelbrotPixelsTemp, width*height*sizeof( unsigned int));
        
        delete newSlice;
        
        canReuseFrame = true;
    }
    
    
    upperImag -= stepSize;
    lowerImag -= stepSize;
}

void Mandelbrot::moveFrameLeft(){
    double frameWidth = rightReal - leftReal;
    double stepSize = frameWidth * moveSpeed;
    
    
    if(moveWithSlices){
        int pixelSteps = moveSpeed * width;
        
        Mandelbrot* newSlice = new Mandelbrot(pixelSteps, height);
        newSlice->setDimensions(leftReal - stepSize, leftReal, lowerImag, upperImag);
        newSlice->render();
        
        for(int i = 0; i < width; i++){
            for (int j = 0; j < height; j++) {
                if(i >= pixelSteps){
                    mandelbrotFloatTemp[j*width + i] = mandelbrotFloat[j * width + i - pixelSteps];
                    mandelbrotIntTemp[j*width + i] = mandelbrotInt[j* width+ i - pixelSteps];
                    mandelbrotPixelsTemp[j*width + i] = mandelbrotPixels[j* width+ i - pixelSteps];
                }
                else{
                    mandelbrotFloatTemp[j*width + i] =newSlice->mandelbrotFloat[j * newSlice->width + i ];
                    mandelbrotIntTemp[j*width + i] = newSlice->mandelbrotInt[j * newSlice->width + i ];
                    mandelbrotPixelsTemp[j*width + i] = newSlice->mandelbrotPixels[j * newSlice->width + i ];
                }
            }
        }
        
        std::memcpy(mandelbrotFloat, mandelbrotFloatTemp, width*height*sizeof( double));
        std::memcpy(mandelbrotInt, mandelbrotIntTemp, width*height*sizeof( unsigned int));
        std::memcpy(mandelbrotPixels, mandelbrotPixelsTemp, width*height*sizeof( unsigned int));
        
        delete newSlice;
        canReuseFrame = true;
    }
    
    leftReal -= stepSize;
    rightReal -= stepSize;
    
    
    
}

void Mandelbrot::moveFrameRight(){
    double frameWidth = rightReal - leftReal;
    double stepSize = frameWidth * moveSpeed;
    
    
    if(moveWithSlices){
        int pixelSteps = moveSpeed * width;
        
        Mandelbrot* newSlice = new Mandelbrot(pixelSteps, height);
        newSlice->setDimensions(rightReal, rightReal + stepSize, lowerImag, upperImag);
        newSlice->render();
        
        for(int i = 0; i < width; i++){
            for (int j = 0; j < height; j++) {
                if(i < width - pixelSteps){
                    mandelbrotFloatTemp[j*width + i] = mandelbrotFloat[j * width + i + pixelSteps];
                    mandelbrotIntTemp[j*width + i] = mandelbrotInt[j* width+ i + pixelSteps];
                    mandelbrotPixelsTemp[j*width + i] = mandelbrotPixels[j* width+ i + pixelSteps];
                }
                else{
                    mandelbrotFloatTemp[j*width + i] =newSlice->mandelbrotFloat[j * newSlice->width+ i - (width - pixelSteps)];
                    mandelbrotIntTemp[j*width + i] = newSlice->mandelbrotInt[j * newSlice->width+ i - (width - pixelSteps)];
                    mandelbrotPixelsTemp[j*width + i] = newSlice->mandelbrotPixels[j * newSlice->width+ i - (width - pixelSteps)];
                }
            }
        }
        
        std::memcpy(mandelbrotFloat, mandelbrotFloatTemp, width*height*sizeof( double));
        std::memcpy(mandelbrotInt, mandelbrotIntTemp, width*height*sizeof( unsigned int));
        std::memcpy(mandelbrotPixels, mandelbrotPixelsTemp, width*height*sizeof( unsigned int));
        
        delete newSlice;
        
        canReuseFrame = true;
    }
    
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
    
    canReuseFrame = false;
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
    
    canReuseFrame = false;
    
}

void Mandelbrot::reset(){
    leftReal = initialLeftReal;
    rightReal = initialRightReal;
    lowerImag = initialLowerImag;
    upperImag = initialUpperImag;
    
    canReuseFrame = false;
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

bool cardioidTest(double real, double imag){
    double q = (real - 0.25)*(real - 0.25) + imag*imag;
    bool res = q*(q+(real - 0.25)) < 0.25*imag*imag;
    
    return res;
}


bool period2BulbTest(double real, double imag){
    bool res = (real+1)*(real+1)+imag*imag < 1.0/16.0;
    
    return res;
}

inline int Mandelbrot::mirrorPy(int py){
    int zeroPos = upperImag/(upperImag - lowerImag) * height;
    
    int offset = py - zeroPos;
    
    
    return zeroPos - offset ;
}



