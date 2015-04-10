//
//  CApp_OnEvent.cpp
//  FractalBrowser
//
//  Event handler for FractalBrowser.
//
//  Created by Dominik on 3/30/15.
//  Copyright (c) 2015 Team060. All rights reserved.
//

#include "CApp.h"

void CApp::OnEvent(SDL_Event* event) {
    switch (event->type)
    {
        case SDL_MOUSEBUTTONUP:
            //render mandelbrot on right click
            if (event->button.button == SDL_BUTTON_RIGHT)
                mandelbrot->render();
            break;
        case SDL_KEYDOWN:
            switch(event->key.keysym.sym ){
                    
                case SDLK_LEFT:
                    mandelbrot->moveFrameLeft();
                    break;
                case SDLK_RIGHT:
                    mandelbrot->moveFrameRight();
                    break;
                case SDLK_UP:
                    mandelbrot->moveFrameUp();
                    break;
                case SDLK_DOWN:
                    mandelbrot->moveFrameDown();
                    break;
                    
                case SDLK_SPACE:
                    mandelbrot->zoomIn();
                    break;
                case SDLK_v:
                    mandelbrot->zoomOut();
                    break;
                    
                case SDLK_q:
                    mandelbrot->rotateColorFilterLeft();
                    mandelbrot->applyColorFilter();
                    break;
                case SDLK_e:
                    mandelbrot->rotateColorFilterRight();
                    mandelbrot->applyColorFilter();
                    break;
                    
                case SDLK_r:
                    mandelbrot->render();
                    break;
                    
                case SDLK_ESCAPE:
                    Running = false;
                    break;
                    
                default:
                    break;
            }
            break;
            
        case SDL_QUIT:
            Running = false;
            break;
    }
    
}