//
//  CApp_OnInit.cpp
//  FractalBrowser
//
//  Created by Dominik on 3/30/15.
//  Copyright (c) 2015 Team060. All rights reserved.
//

#include "CApp.h"

bool CApp::OnInit() {
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        return false;
    }
    
    window = SDL_CreateWindow("Fractal Browser",
                              SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);
    renderer = SDL_CreateRenderer(window, -1, 0);
    //SDL_CreateWindowAndRenderer(width, height, SDL_WINDOW_RESIZABLE, &window, &renderer);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, width, height);
    pixels = new Uint32[width * height];
    memset(pixels, 255, width * height * sizeof(Uint32));
    
    mandelbrot = new Mandelbrot(width, height);
    
    
    return true;
}