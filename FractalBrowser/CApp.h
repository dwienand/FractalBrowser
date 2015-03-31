//
//  CApp.h
//  FractalBrowser
//
//  Created by Dominik on 3/30/15.
//  Copyright (c) 2015 Team060. All rights reserved.
//

#ifndef _CAPP_H_
#define _CAPP_H_


#include <SDL2/SDL.h>
#include "Mandelbrot.h"

class CApp {
private:
    bool            Running;
    
    SDL_Window*    window;
    SDL_Renderer* renderer;
    SDL_Event event;
    bool leftMouseButtonDown = false;
    
    SDL_Texture* texture;
    
    int width = 800, height=600;
    
    Uint32 * pixels;
    
    Mandelbrot* mandelbrot;
    
public:
    CApp();
    
    int OnExecute();
    
public:
    bool OnInit();
    
    void OnEvent(SDL_Event* Event);
    
    void OnLoop();
    
    void OnRender();
    
    void OnCleanup();
};

#endif
