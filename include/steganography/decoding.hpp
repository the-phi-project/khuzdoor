// The Phi Project
// Copyright 2026 Khuzdoor Authors
// Use of this source code is governed by the GPL-3.0 license, found in the LICENSE file
//
// Date created: 02-04-2026
// Original author: Zevi Berlin (zeviberlin@gmail.com)
// Latest editor: Zevi Berlin (zeviberlin@gmail.com)

#ifndef DECODING_HPP
#define DECODING_HPP

#include <string>
#include <vector>
#include <cstdint>

#include "steganography/Image.hpp"
#include "steganography/lsbmr.hpp"
#include "steganography/encoding.hpp"
#include "encryption/random.hpp"

//---------> [ Config. Separator ] <---------\\ 

namespace khuzdoor::steg {

/// Regenerate the deterministically random pixel
/// indices at which data was encoded, then read
/// from those images, DOES NOT decrypt/decompress
/// @param img image wrapper class which contains host image data
/// @param password the password, in order to regenerate indices
std::string decodeRandomLSBMR(const Image& img, const std::string& password);

//================={ Header Item Separator }=================\\ 

/// Encode secret bits into edge pixels of an image, this
/// is quite resistant to statistical steganalysis because
/// edge data has higher entropy so stored data looks less
/// suspicious from a mathematical perspective
/// @param img image wrapper class which contains host image data
std::string decodeEdgeLSBMR(const Image& img);

//================={ Header Item Separator }=================\\ 

/// Encode secret bits into texture pixels of an image, this
/// is quite resistant to statistical steganalysis because
/// textured data has higher entropy so stored data looks less
/// suspicious from a mathematical perspective
/// @param img image wrapper class which contains host image data
std::string decodeTextureLSBMR(const Image& img);

}  // namespace khuzdoor::steg

#endif /* DECODING_HPP */