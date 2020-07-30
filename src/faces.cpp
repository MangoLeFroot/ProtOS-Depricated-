#include "faces.h"

using json = nlohmann::json;

Faces::Faces() {

}

Faces::~Faces() {
    for (Face face : faces) {
        face.dflt.free();
        face.blink.free();
    }
}

bool Faces::load(const std::string &name) {
    std::ifstream stream(name);
    if(!stream)
        return false;

    json j;
    stream >> j;
    stream.close();

    for(auto& face : j) {
        // Load face options from config
        Face f;
        f.name = face["name"];
        f.path = face["path"];
        f.color = rgb_matrix::Color(face["color"][0], face["color"][1], face["color"][2]);

        // Preload all images
        Image dflt;
        dflt.load(f.path);
        f.dflt = dflt;

        faces.push_back(f);
    }

    return true;
}

void Faces::show(const std::string &name) {
    auto it = std::find_if(faces.begin(), faces.end(), [name](const Face& face) { return face.name == name; });
    
    if (it == faces.end()) {
        fprintf(stderr, "Face (%s) not found.\n", name.c_str());
        return;
    }

    current = &*it;
}

void Faces::update() {
    if (current == nullptr)
        return;
}

void Faces::draw(rgb_matrix::FrameCanvas *canvas) {
    if (canvas == nullptr)
        return;

    if (current == nullptr)
        return;

    for(int y = 0; y < current->dflt.height(); y++) {
        for(int x = 0; x < current->dflt.width(); x++) {
            canvas->SetPixel(x, y, current->dflt.frames()[0][x + (y * current->dflt.width())].r,
                                   current->dflt.frames()[0][x + (y * current->dflt.width())].g,
                                   current->dflt.frames()[0][x + (y * current->dflt.width())].b);

            canvas->SetPixel(127 - (x + 64) + 64, y, current->dflt.frames()[0][x + (y * current->dflt.width())].r,
                                                     current->dflt.frames()[0][x + (y * current->dflt.width())].g,
                                                     current->dflt.frames()[0][x + (y * current->dflt.width())].b);
        }
    }
}