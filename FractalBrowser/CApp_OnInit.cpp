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
    

    if(this->retinaSupported){
        SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "0");
        width *= 2;
        height *= 2;
    }
    
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        return false;
    }

    if(retinaSupported){
    window = SDL_CreateWindow("Fractal Browser",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width / 2, height / 2,
                              SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
    }else{
        window = SDL_CreateWindow("Fractal Browser",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height,
                                  SDL_WINDOW_SHOWN);
    }
    
    renderer = SDL_CreateRenderer(window, -1, 0);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, width, height);
    
    int w, h;
    SDL_GL_GetDrawableSize(window, &w, &h);
    std::cout << "Drawable:" << w << "x" << h << std::endl;
    
    LOG(INFO) << "Sucessfully intialized SDL";
    
    LOG(INFO) << "READ ME: \n navigate x/y plane with arrow keys, zoom in with space and out with 'v' rotate color filters with 'q' and 'e'\n hit escape to quit";
    
    //create array for rendering into
    pixels = new Uint32[width * height];
    
    //set it to be all black initially
    memset(pixels, 0, width * height * sizeof(Uint32));
    
    //create new mandelbrot to render
    mandelbrot = new Mandelbrot(width, height);
    
    pixels = mandelbrot->getMandelbrotPixels();
    mandelbrot->render();
    
    
    return true;
}