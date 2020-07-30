#pragma once

#include "pch.h"

enum IMAGE_FLIP {
    HORIZONTAL,
    VERTICAL
};

class Image {
public:
 
    Image();

    void load(const std::string& path);
    void flip(IMAGE_FLIP flip);
    void free();

    const int& width() const { return _width; }
    const int& height() const { return _height; }
    const std::vector<rgb_matrix::Color*>& frames() const { return _frames; }

private:
    int _width;
    int _height;
    std::vector<rgb_matrix::Color*> _frames;

    void LoadBMP(const std::string& path);
    void LoadGIF(const std::string& path);
};