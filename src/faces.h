#pragma once

#include "pch.h"
#include "image.h"

struct Face {
    std::string name;
    std::string path;
    rgb_matrix::Color color;
    Image dflt;
    Image blink;
};

class Faces {
public:
    Faces();
    ~Faces();
    
    bool load(const std::string &name);
    void show(const std::string &name);
    void update();
    void draw(rgb_matrix::FrameCanvas *canvas);

private:
    std::vector<Face> faces;
    Face *current;
};