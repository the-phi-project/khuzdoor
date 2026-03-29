// The Phi Project
// Copyright 2026 Khuzdoor Authors
// Use of this source code is governed by the GPL-3.0 license, found in the LICENSE file
//
// Date created: 09-03-2026
// Original author: Zevi Berlin (zeviberlin@gmail.com)
// Latest editor: Zevi Berlin (zeviberlin@gmail.com)

#include "steganography/lsbmr.hpp"

#include <cstddef>
#include <string>
#include <optional>
#include <span>
#include <vector>
#include <bitset>
#include <cstdint>

#include <sodium.h>

//---------> [ Config. Separator ] <---------\\ 

void khuzdoor::steg::writeLSBMR(uint8_t* pixel, bool bit) {
  if ((*pixel & 1U) == (uint8_t)bit) {  // pixel's LSB already is bit
    return;                             // no change necessary
  }

  // basically just a pseudo-random way to determine +/-
  if (randombytes_random() % 2 == 0) {
    *pixel = (*pixel == 255) ? 254 : (*pixel) - 1;  // avoid overflow
  } else {
    *pixel = (*pixel == 0) ? 1 : (*pixel) + 1;  // avoid underflow
  }
}

//------------[ Func. Implementation Separator ]------------\\ 

bool khuzdoor::steg::readLSBMR(const uint8_t* pixel) {
  return (*pixel & 1U) != 0;
}