// The Phi Project
// Copyright 2026 Khuzdoor Authors
// Use of this source code is governed by the GPL-3.0 license, found in the LICENSE file
//
// Date created: 09-03-2026
// Original author: Zevi Berlin (zeviberlin@gmail.com)
// Latest editor: Zevi Berlin (zeviberlin@gmail.com)

#include "encryption/steg.hpp"

#include <string>
#include <optional>
#include <span>
#include <vector>
#include <bitset>
#include <cstdint>

#include <magic.h>

//---------> [ Config. Separator ] <---------\\ 

void khuzdoor::encryption::writeLengthBytes(uint32_t length, uint8_t* pixels, int channels) {
  // endian-ness doesnt matter because this program is consistent with byte-order
  std::bitset<8> len_bytes[]{
    (uint8_t)(length >> 24),           //
    (uint8_t)((length >> 16) & 0xFF),  //
    (uint8_t)((length >> 8) & 0xFF),   //
    (uint8_t)(length & 0xFF)           //
  };

  /**/

  int byte = 0, bit = 0;
  for (uint8_t* pixel = pixels; pixel < (pixels + (32 * channels)); pixel += channels) {
    if (len_bytes[byte][7 - bit]) {
      *pixel |= 1U;  // set last bit to 1
    } else {
      *pixel &= ~(1U);  // set last bit to 0
    }

    if (bit == 7) {
      byte++;
      bit = 0;
    } else {
      bit++;
    }
  }
}

//------------[ Func. Implementation Separator ]------------\\ 

uint32_t khuzdoor::encryption::readLengthBytes(uint8_t* pixels, int channels) {
  uint32_t length = 0;

  for (uint8_t* pixel = pixels; pixel < (pixels + (32 * channels)); pixel += channels) {
    length <<= 1;
    length &= ~(1U);
    if ((*pixel & 1U) != 0) length++;
  }

  return length;
}