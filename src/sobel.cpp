// The Phi Project
// Copyright 2026 Khuzdoor Authors
// Use of this source code is governed by the GPL-3.0 license, found in the LICENSE file
//
// Date created: 05-04-2026
// Original author: Zevi Berlin (zeviberlin@gmail.com)
// Latest editor: Zevi Berlin (zeviberlin@gmail.com)

#include "steganography/sobel.hpp"

#include <array>
#include <cstdint>
#include <cmath>  // ::sqrt(), std::round()

#include "Image.hpp"

//---------> [ Config. Separator ] <---------\\ 

khuzdoor::steg::ImageData khuzdoor::steg::RGB2GS_image(const khuzdoor::steg::Image& img) {
  khuzdoor::steg::ImageData grayscale_img(img.getWidth(), img.getHeight(), 1);

  for (uint32_t i = 0; i < img.getWidth() * img.getHeight(); i++) {
    uint8_t red = *img.at(i * img.getChannels());
    uint8_t green = *img.at((i * img.getChannels()) + 1);
    uint8_t blue = *img.at((i * img.getChannels()) + 2);

    grayscale_img.data[i] = khuzdoor::steg::RGB2GS_pixel(red, green, blue);
  }

  return grayscale_img;
}

//------------[ Func. Implementation Separator ]------------\\ 

uint16_t khuzdoor::steg::calculateSobels(const uint8_t matrix[SOBEL_X][SOBEL_Y]) {
  int16_t G_x = (matrix[0][0] * -1) + /*(matrix[0][1] * 0) + */ (matrix[0][2] * +1) +
                (matrix[1][0] * -2) + /*(matrix[1][1] * 0) + */ (matrix[1][2] * +2) +
                (matrix[2][0] * -1) + /*(matrix[2][1] * 0) + */ (matrix[2][2] * +1);

  int16_t G_y = (matrix[0][0] * -1) + (matrix[0][1] * -2) + (matrix[0][2] * -1) +
                /* (matrix[1][0] * 0) + (matrix[1][1] * 0) + (matrix[1][2] * 0) + */
                (matrix[2][0] * +1) + (matrix[2][1] * +2) + (matrix[2][2] * +1);

  return static_cast<uint16_t>(
    std::round(::sqrt(static_cast<double>((G_x * G_x)) + (G_y * G_y))));  // slow but accurate
}