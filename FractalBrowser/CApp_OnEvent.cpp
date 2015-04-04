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
                pixels = mandelbrot->render();
            break;
        case SDL_KEYDOWN:
            switch(event->key.keysym.sym ){
                case SDLK_LEFT:
                    mandelbrot->moveFrameLeft();
                    pixels = mandelbrot->render();
                    break;
                case SDLK_RIGHT:
                    mandelbrot->moveFrameRight();
                    pixels = mandelbrot->render();
                    break;
                case SDLK_UP:
                    mandelbrot->moveFrameUp();
                    pixels = mandelbrot->render();
                    break;
                case SDLK_DOWN:
                    mandelbrot->moveFrameDown();
                    pixels =mandelbrot->render();
                    break;
                case SDLK_SPACE:
                    mandelbrot->zoomIn();
                    pixels = mandelbrot->render();
                    break;
                case SDLK_v:
                    mandelbrot->zoomOut();
                    pixels = mandelbrot->render();
                    break;
                case SDLK_q:
                    mandelbrot->rotateColorFilterLeft();
                    break;
                case SDLK_e:
                    mandelbrot->rotateColorFilterRight();
                    break;
                    
                case SDLK_r:
                    mandelbrot->render();
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