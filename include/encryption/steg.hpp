// The Phi Project
// Copyright 2026 Khuzdoor Authors
// Use of this source code is governed by the GPL-3.0 license, found in the LICENSE file
//
// Date created: 09-03-2026
// Original author: Zevi Berlin (zeviberlin@gmail.com)
// Latest editor: Zevi Berlin (zeviberlin@gmail.com)

#ifndef STEG_HPP
#define STEG_HPP

#include <string>
#include <optional>
#include <span>
#include <vector>
#include <bitset>
#include <cstdint>

#include <magic.h>

//---------> [ Config. Separator ] <---------\\ 

namespace khuzdoor::encryption {

// write the length of the content to the image pixel buffer
// length: the length (of the data) to write
// pixels: image pixel buffer pointer
// channels: number of channels, important for determining stride
void writeLengthBytes(uint32_t length, uint8_t* pixels, int channels);

//================={ Header Item Separator }=================\\ 

// read the first four bytes of the image pixel buffer to
// get the length
// pixels: image pixel buffer pointer
// channels: number of channels, important for determining stride
uint32_t readLengthBytes(uint8_t* pixels, int channels);

}  // namespace khuzdoor::encryption

#endif /* STEG_HPP */