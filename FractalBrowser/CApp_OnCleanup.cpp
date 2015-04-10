//
//  CApp_OnCleanup.cpp
//  FractalBrowser
//
//  Clean-up SDL app
//
//  Created by Dominik on 3/30/15.
//  Copyright (c) 2015 Team060. All rights reserved.
//

#include "CApp.h"

void CApp::OnCleanup() {
    LOG(INFO) << "Tearing down FractalBrowser ...";
    
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    delete this->mandelbrot;
    LOG(INFO) << "Terminated FractalBrowser";
}