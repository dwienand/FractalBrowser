//
//  BMPReader.cpp
//  FractalBrowser
//
//  Created by Dominik Wienand on 4/10/15.
//  Copyright (c) 2015 Team060. All rights reserved.
//

#include "BMPReader.h"
using std::vector;

vector<unsigned int>* ReadBMP(const char* filename)
{
    FILE* f = fopen(filename, "rb");
    
    if(f == NULL)
        throw "File not found!";
    
    unsigned char info[54];
    fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header
    
    // extract image height and width from header
    int width = *(int*)&info[18];
    
    int row_padded = (width*3 + 3) & (~3);
    unsigned char* data = new unsigned char[row_padded];
    
    fread(data, sizeof(unsigned char), row_padded, f);
    
    vector<unsigned int>* palette = new vector<unsigned int>(0);
    for(int j = 0; j < width*3; j += 3)
    {
        unsigned int color = data[j] | (data[j+1] << 8) | (data[j+2] << 16);
        palette->push_back(color);
    }
    
    delete data;
    
    fclose(f);
    return palette;
}