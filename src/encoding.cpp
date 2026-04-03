// The Phi Project
// Copyright 2026 Khuzdoor Authors
// Use of this source code is governed by the GPL-3.0 license, found in the LICENSE file
//
// Date created: 02-04-2026
// Original author: Zevi Berlin (zeviberlin@gmail.com)
// Latest editor: Zevi Berlin (zeviberlin@gmail.com)

#include "steganography/encoding.hpp"

#include <string>
#include <vector>
#include <cstdint>

#include "steganography/Image.hpp"
#include "steganography/lsbmr.hpp"
#include "encryption/random.hpp"

//---------> [ Config. Separator ] <---------\\ 

void khuzdoor::steg::encodeRandomLSBMR(const khuzdoor::steg::Image& img, const std::string& data,
                                       const std::string& password) {
  uint32_t length = data.length();

  const std::vector<uint32_t> length_indices = khuzdoor::encryption::seededRandomIndices(
    0, sizeof(uint32_t), sizeof(uint32_t) * 8 * 6, password + "$$length");

  for (uint32_t i = 0; i < length_indices.size(); i++) {
    khuzdoor::steg::writeLSBMR(img.at(length_indices[i] * img.getChannels()),
                               ((length >> (31 - i)) & 1U) != 0);
    // shift by `31 - i` because shifting by 32 is undefined behavior
    // same as using `7 - bit` below
  }

  //~~~~~~~< Misc. Separator >~~~~~~~\\ 

  // BUG FIX: Not `* img.getChannels()` because thats done already below
  const std::vector<uint32_t> data_indices = khuzdoor::encryption::seededRandomIndices(
    (sizeof(uint32_t) * 8 * 6) + 1, length, img.getWidth() * img.getHeight(), password + "$$data");


  uint32_t byte = 0;
  uint8_t bit = 0;
  for (uint32_t i = 0; i < data_indices.size(); i++) {
    khuzdoor::steg::writeLSBMR(img.at(data_indices[i] * img.getChannels()),
                               (((uint8_t)data[byte] >> (7 - bit)) & 1U) > 0);

    // clang-format off
    if (bit == 7) { byte++; bit = 0; }
    else { bit++; }
    // clang-format on
  }
}