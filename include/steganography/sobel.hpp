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

********************************************************/

#include <array>
#include <cstdint>
#include <cmath>  // ::abs(), std::round()

#include "Image.hpp"

#define SOBEL_MATRIX_SIZE_X 3  // edge matrix will be
#define SOBEL_MATRIX_SIZE_Y 3  // 3x3, as seen above

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

}  // namespace khuzdoor::steg

#endif /* SOBEL_HPP */