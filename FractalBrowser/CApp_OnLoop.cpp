//
//  CApp_OnLoop.cpp
//  FractalBrowser
//
//  Created by Dominik on 3/30/15.
//  Copyright (c) 2015 Team060. All rights reserved.
//


#include "CApp.h"

void CApp::OnLoop() {
    SDL_UpdateTexture(texture, NULL, pixels, width * sizeof(Uint32));
    
}