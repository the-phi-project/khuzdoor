// The Phi Project
// Copyright 2026 Khuzdoor Authors
// Use of this source code is governed by the GPL-3.0 license, found in the LICENSE file
//
// Date created: 05-04-2026
// Original author: Zevi Berlin (zeviberlin@gmail.com)
// Latest editor: Zevi Berlin (zeviberlin@gmail.com)

#ifndef SOBEL_HPP
#define SOBEL_HPP

/************** SOBEL'S OPERATOR EXPLAINED **************
Sobel's operator uses a 3x3 convolution matrix on image
pixels to calculate the gradient of that matrix, and
based on a threshold decides if the pixels are an edge
  for example:
    [ -1  0  1 ]         [ -1  -2  -1 ]
    [ -2  0  2 ] = G_x   [  0   0   0 ] = G_y
    [ -1  0  1 ]         [  1   2   1 ]

We first take a 3x3 matrix of pixels, then multiply their
grayscale values by the coefficients in the operator's
matrix, then we can calculate the gradient with either
G = sqrt(G_x^2 + G_y^2)  OR  G = |G_x| + |G_y| (faster)
Then, we check G >= threshold ? edge : not edge

https://en.wikipedia.org/wiki/Sobel_operator
********************************************************/

#include <array>
#include <cstdint>
#include <cmath>  // ::sqrt(), std::round()

#include "Image.hpp"

#define SOBEL_X 3           // edge matrix will be
#define SOBEL_Y 3           // 3x3, as seen above
#define EDGE_THRESHOLD 118  // the gradient of a sobel array must be >= 118 to be an edge

//---------> [ Config. Separator ] <---------\\ 

namespace khuzdoor::steg {

/// Convert an RGB pixel to a grayscale one
/// @param red the 0..255 value for red
/// @param green the 0..255 value for red
/// @param blue the 0..255 value for red
/// @returns the 0..255 value for grayscale intensity
/// @note relies on the linear tranformation defined in
///       the SO answer https://stackoverflow.com/a/17619494
inline uint8_t RGB2GS_pixel(uint8_t red, uint8_t green, uint8_t blue) {
  double lum = (0.2126 * red) + (0.7152 * green) + (0.0722 * blue);
  return static_cast<uint8_t>(std::round(lum));
}

/// Convert an entire RGB image to a grayscale image
/// @param img the RGB image
/// @returns the ImageData struct for the grayscale image
ImageData RGB2GS_image(const Image& img);

/// Take in a 2D array and return the sobel
uint16_t calculateSobels(const uint8_t matrix[SOBEL_X][SOBEL_Y]);

}  // namespace khuzdoor::steg

#endif /* SOBEL_HPP */