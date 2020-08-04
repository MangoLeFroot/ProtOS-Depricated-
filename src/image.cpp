#include "image.h"
#include "image_headers.h"

Image::Image() {

}

void Image::load(const std::string& path) {
    std::string ext = path.substr(path.find_last_of(".") + 1);

    if(!strcmp(ext.c_str(), "bmp")) {
        LoadBMP(path);
    } else if (!strcmp(ext.c_str(), "gif")) {
        LoadGIF(path);
    } else {
        fprintf(stderr, "Unknown extension (%s), only bmp and gif are supported.\n", ext.c_str());
    }
}

void Image::flip(IMAGE_FLIP flip) {
    for(rgb_matrix::Color* frame : _frames) {
        if(flip == IMAGE_FLIP::HORIZONTAL) {
            for(int y = 0; y < _height; y++) {
                for(int x = _width / 2; x > 0; --x) {
                    std::swap(frame[(x - 1) + (y * _width)], frame[(_width - x) + (y * _width)]);
                }
            }
        } else if (flip == IMAGE_FLIP::VERTICAL) {
            for(int y = _height / 2; y > 0; --y) {
                for(int x = 0; x < _width; x++) {
                    std::swap(frame[x + ((y - 1) * _width)], frame[x + ((_height - y) * _width)]);
                }
            }
        }
    }
}

void Image::free() {
    for(rgb_matrix::Color* frame : _frames) {
        if(frame) {
            delete[] frame;
            frame = nullptr;
        }
    }
}

void Image::LoadBMP(const std::string& path) {
    BITMAPFILEHEADER bmfh;
    BITMAPINFOHEADER bmih;
    FILE*            file;

    file = fopen(path.c_str(), "rb");
    if(file == nullptr) {
        fprintf(stderr, "Failed to open %s.\n", path.c_str());
        return;
    }

    // Load bitmap headers
    fread(&bmfh, sizeof(BITMAPFILEHEADER), 1, file);
    fread(&bmih, sizeof(BITMAPINFOHEADER), 1, file);

    if(reinterpret_cast<char*>(&bmfh.bfType)[0] != 'B' ||
       reinterpret_cast<char*>(&bmfh.bfType)[1] != 'M') {
        fprintf(stderr, "Bitmap (%s) in invalid format.\n", path.c_str());
        fclose(file);
        return;
    }

    _width = bmih.biWidth;
    _height = abs(bmih.biHeight);

    // Set to position data offset for image
    fseek(file, bmfh.bfOffBits, SEEK_SET);

    rgb_matrix::Color* pixels = new rgb_matrix::Color[_width * _height];

    if (bmih.biBitCount == 32) {
        for(int y = 0; y < _height; y++) {
            for(int x = 0; x < _width; x++) {
                char pixel[4];
                fread(&pixel, sizeof(pixel), 1, file);

                pixels[x + (y * _width)].r = pixel[2];
                pixels[x + (y * _width)].g = pixel[1];
                pixels[x + (y * _width)].b = pixel[0];
            }
        }
    } else {
        fprintf(stderr, "(%s): Invalid bit count.\n", path.c_str());
        delete[] pixels;
        fclose(file);
        return;
    }

    _frames.push_back(pixels);

    fclose(file);

    if(bmih.biHeight > 0) {
        flip(IMAGE_FLIP::VERTICAL);
    }
}

void Image::LoadGIF(const std::string& path) {
    
}