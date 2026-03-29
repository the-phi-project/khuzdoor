// The Phi Project
// Copyright 2026 Khuzdoor Authors
// Use of this source code is governed by the GPL-3.0 license, found in the LICENSE file
//
// Date created: 09-03-2026
// Original author: Zevi Berlin (zeviberlin@gmail.com)
// Latest editor: Zevi Berlin (zeviberlin@gmail.com)

#ifndef LSBM_HPP
#define LSBM_HPP

#include <string>
#include <optional>
#include <span>
#include <vector>
#include <bitset>
#include <cstdint>

#include <sodium.h>

//---------> [ Config. Separator ] <---------\\ 

namespace khuzdoor::steg {

/// writes a secret bit to a pixel's LSB from a pixel array
/// via the `stbi_load()` method. LSBM is when if the pixel's LSB
/// does not already match the bit to write, it randomly adds or
/// subtracts 1 (to make pixel match) such that it makes the changed
/// data look more random
/// @param pixel pointer to a pixel in the pixel array
/// @param bit whether the bit should be set to 1 or 0
void writeLSBMR(uint8_t* pixel, bool bit);

//================={ Header Item Separator }=================\\ 

/// reads the secret bit from a pixel's LSB from a pixel array
/// via the `stbi_load()` method
/// @param pixel pointer to a pixel in the pixel array
/// @returns whether the bit is 1 or 0
bool readLSBMR(const uint8_t* pixel);

}  // namespace khuzdoor::steg

#endif /* LSBM_HPP */