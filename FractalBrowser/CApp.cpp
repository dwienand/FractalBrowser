//
//  CApp.cpp
//  FractalBrowser
//
// Main class for FractalBrowser
//
//  Created by Dominik on 3/30/15.
//  Copyright (c) 2015 Team060. All rights reserved.
//

#include "CApp.h"

CApp::CApp() {
    window = NULL;
    
    Running = true;
}

int CApp::OnExecute() {
    if(OnInit() == false) {
        return -1;
    }
    
    SDL_Event event;
    
    while(Running) {
        OnLoop();
        OnRender();
        SDL_WaitEvent(&event);
        OnEvent(&event);
        
       
    }
    
    OnCleanup();
    
    return 0;
}

int main(int argc, char* argv[]) {
    CApp theApp;
    
    return theApp.OnExecute();
}