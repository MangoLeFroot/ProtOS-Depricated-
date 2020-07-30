#include <signal.h>

#include "pch.h"

#include "faces.h"

using rgb_matrix::GPIO;
using rgb_matrix::Canvas;
using rgb_matrix::FrameCanvas;
using rgb_matrix::RGBMatrix;

volatile bool interrupt_received = false;
static void InterruptHandler(int signo) {
  interrupt_received = true;
}

RGBMatrix *CreateMatrix();

int main() {
  Faces faces;
  if(!faces.load("./faces.json")) {
    fprintf(stderr, "Failed to load faces config.\n");
    return EXIT_FAILURE;
  }
  
  faces.show("mango");

  // Configure and initialize the matrices.
  RGBMatrix *matrix = CreateMatrix();
  if (matrix == nullptr) {
    fprintf(stderr, "Failed to initialize matrices.\n");
    return EXIT_FAILURE;
  }

  FrameCanvas *canvas = matrix->CreateFrameCanvas();

  // setup signal processing
  signal(SIGTERM, InterruptHandler);
  signal(SIGINT, InterruptHandler);

  // main loop
  do {
    // Updates
    faces.update();

    //Draw stuff
    faces.draw(canvas);

    canvas = matrix->SwapOnVSync(canvas);  
  } while(!interrupt_received);

  // shutdown and cleanup
  matrix->Clear();
  delete matrix;

  return EXIT_SUCCESS;
}

RGBMatrix *CreateMatrix() {
  RGBMatrix::Options matrix_options;
  rgb_matrix::RuntimeOptions runtime_opt;
  
  matrix_options.hardware_mapping = "adafruit-hat-pwm";
  matrix_options.cols = 64;
  matrix_options.rows = 32;
  matrix_options.chain_length = 2;
  matrix_options.parallel = 1;

  matrix_options.brightness = 80;
  matrix_options.led_rgb_sequence = "BRG";
  matrix_options.limit_refresh_rate_hz = 90;

  //matrix_options.show_refresh_rate = true;

  runtime_opt.gpio_slowdown = 2;
  runtime_opt.drop_privileges = 0;

  return CreateMatrixFromOptions(matrix_options, runtime_opt);
}