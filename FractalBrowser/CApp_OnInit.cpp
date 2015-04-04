//
//  CApp_OnInit.cpp
//  FractalBrowser
//
//  Created by Dominik on 3/30/15.
//  Copyright (c) 2015 Team060. All rights reserved.
//

#include "CApp.h"

bool CApp::OnInit() {
    LOG(INFO) << "Initializing SDL ...";
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        return false;
    }

    window = SDL_CreateWindow("Fractal Browser",
                              SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);
    renderer = SDL_CreateRenderer(window, -1, 0);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, width, height);
    
    LOG(INFO) << "Sucessfully intialized SDL";
    
    //create array for rendering into
    pixels = new Uint32[width * height];
    
    //set it to be all white initially
    memset(pixels, 255, width * height * sizeof(Uint32));
    
    //create new mandelbrot to render
    mandelbrot = new Mandelbrot(width, height);
    
    pixels = mandelbrot->render();
    
    
    return true;
}