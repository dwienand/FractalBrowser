//
//  CApp_OnEvent.cpp
//  FractalBrowser
//
//  Created by Dominik on 3/30/15.
//  Copyright (c) 2015 Team060. All rights reserved.
//

#include "CApp.h"

void CApp::OnEvent(SDL_Event* event) {
    switch (event->type)
    {
        case SDL_MOUSEBUTTONUP:
            if (event->button.button == SDL_BUTTON_RIGHT)
                pixels = mandelbrot->render();
            break;
            
        case SDL_QUIT:
            Running = false;
            break;
    }
    
}