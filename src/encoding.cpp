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

#include "Image.hpp"
#include "steganography/lsbmr.hpp"
#include "steganography/sobel.hpp"
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

//------------[ Func. Implementation Separator ]------------\\ 
//------------[ Func. Implementation Separator ]------------\\ 

#define EXP3(name) name[0], name[1], name[2]  // EXPAND 3 ITEM ARRAYS

// TODO: handle channels
std::vector<uint32_t> khuzdoor::steg::edgeEncodable(const khuzdoor::steg::ImageData& grayscale,
                                                    uint32_t length) {
  std::vector<uint32_t> edges{};

  // grayscale image has only 1 channel
  for (uint32_t i = 1; i < (grayscale.width * grayscale.height) - 1; i++) {
    uint8_t row1[]{grayscale.data[i - 1], grayscale.data[i], grayscale.data[i + 1]};

    // `* grayscale.width` because thats the stride and accesses the next row
    uint8_t row2[]{grayscale.data[(i - 1) + (grayscale.width)],
                   grayscale.data[i + (grayscale.width)],
                   grayscale.data[(i + 1) + (grayscale.width)]};

    uint8_t row3[]{grayscale.data[(i - 1) + (grayscale.width * 2)],
                   grayscale.data[i + (grayscale.width * 2)],
                   grayscale.data[(i + 1) + (grayscale.width * 2)]};

    uint8_t rows[][3]{{EXP3(row1)}, {EXP3(row2)}, {EXP3(row3)}};
    uint16_t sobels = khuzdoor::steg::calculateSobels(rows);

    if (sobels >= EDGE_THRESHOLD) {
      edges.push_back(i - 1);
      edges.push_back(i);
      edges.push_back(i + 1);
    }
  }

  return (edges.size() >= (size_t)length * 8) ? edges : std::vector<uint32_t>{};
}

//------------[ Func. Implementation Separator ]------------\\ 

void khuzdoor::steg::encodeEdgeLSBMR(const khuzdoor::steg::Image& img,
                                     std::vector<uint32_t>& possible_indices,
                                     const std::string& data, const std::string& password) {
  uint32_t length = data.length();
  khuzdoor::encryption::deterministicShuffle(possible_indices, password);  // edits in place: O(1)

  for (uint32_t i = 0; i < sizeof(uint32_t) * 8; i++) {
    khuzdoor::steg::writeLSBMR(img.at(possible_indices[i] * img.getChannels()),
                               ((length >> (31 - i)) & 1U) != 0);
    // shift by `31 - i` because shifting by 32 is undefined behavior
    // same as using `7 - bit` below
  }

  //~~~~~~~< Misc. Separator >~~~~~~~\\

  uint32_t byte = 0;
  uint8_t bit = 0;
  for (uint32_t i = (sizeof(uint32_t) * 8) + 1; i < data.size() * 8; i++) {
    khuzdoor::steg::writeLSBMR(img.at(possible_indices[i] * img.getChannels()),
                               (((uint8_t)data[byte] >> (7 - bit)) & 1U) > 0);

    // clang-format off
    if (bit == 7) { byte++; bit = 0; }
    else { bit++; }
    // clang-format on
  }
}