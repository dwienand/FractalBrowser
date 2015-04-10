//
//  BMPReader.h
//  FractalBrowser
//
//  Created by Dominik Wienand on 4/10/15.
//  Copyright (c) 2015 Team060. All rights reserved.
//

#ifndef __FractalBrowser__BMPReader__
#define __FractalBrowser__BMPReader__

#include <vector>
using std::vector;
vector<unsigned int>* ReadBMP(const char* filename);
#endif /* defined(__FractalBrowser__BMPReader__) */
