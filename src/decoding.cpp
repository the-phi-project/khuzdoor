// The Phi Project
// Copyright 2026 Khuzdoor Authors
// Use of this source code is governed by the GPL-3.0 license, found in the LICENSE file
//
// Date created: 02-04-2026
// Original author: Zevi Berlin (zeviberlin@gmail.com)
// Latest editor: Zevi Berlin (zeviberlin@gmail.com)

#include "steganography/decoding.hpp"

#include <string>
#include <vector>
#include <cstdint>

#include "steganography/Image.hpp"
#include "steganography/lsbmr.hpp"
#include "encryption/random.hpp"

//---------> [ Config. Separator ] <---------\\ 

std::string khuzdoor::steg::decodeRandomLSBMR(const Image& img, const std::string& password) {
  uint32_t length = 0;

  const std::vector<uint32_t> length_indices = khuzdoor::encryption::seededRandomIndices(
    0, sizeof(uint32_t), sizeof(uint32_t) * 8 * 6, password + "$$length");

  for (uint32_t i = 0; i < length_indices.size(); i++) {
    length <<= 1;
    length += (uint32_t)khuzdoor::steg::readLSBMR(img.at(length_indices[i] * img.getChannels()));
  }

  //~~~~~~~< Misc. Separator >~~~~~~~\\ 

  // BUG FIX: Not `* img.getChannels()` because thats done already below
  const std::vector<uint32_t> data_indices = khuzdoor::encryption::seededRandomIndices(
    (sizeof(uint32_t) * 8 * 6) + 1, length, img.getWidth() * img.getHeight(), password + "$$data");

  std::vector<char> data(length);
  uint32_t byte = 0;
  uint8_t bit = 0;
  for (uint32_t i = 0; i < data_indices.size(); i++) {
    data[byte] <<= 1;
    data[byte] += khuzdoor::steg::readLSBMR(img.at(data_indices[i] * img.getChannels()));  // NOLINT
    // ^ this is fine because it returns a bool which is either 1 or 0

    // clang-format off
    if (bit == 7) { byte++; bit = 0; }
    else { bit++; }
    // clang-format on
  }

  return std::string(data.begin(), data.end());
}