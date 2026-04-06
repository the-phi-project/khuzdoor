// The Phi Project
// Copyright 2026 Khuzdoor Authors
// Use of this source code is governed by the GPL-3.0 license, found in the LICENSE file
//
// Date created: 02-04-2026
// Original author: Zevi Berlin (zeviberlin@gmail.com)
// Latest editor: Zevi Berlin (zeviberlin@gmail.com)

#ifndef ENCODING_HPP
#define ENCODING_HPP

#include <string>
#include <vector>
#include <cstdint>

#include "Image.hpp"
#include "steganography/lsbmr.hpp"
#include "steganography/sobel.hpp"
#include "encryption/random.hpp"

//---------> [ Config. Separator ] <---------\\ 

namespace khuzdoor::steg {

/// Instead of a "smart" encoding mechanism, generate
/// deterministically random indices throughout the image
/// data, pixels at those indices will have secret bits encoded into
/// them, less resistant to RS and Chi^2 than others
/// @param img image wrapper class which contains host image data
/// @param data the data to write to the host image
/// @param password the password, in order to regenerate indices
void encodeRandomLSBMR(const Image& img, const std::string& data, const std::string& password);

//================={ Header Item Separator }=================\\ 

/// Check whether or not an image has enough edge pixels
/// such that all of the secret BITS can be encoded
/// @param img image wrapper class which contains host image data
/// @param length length IN BYTES of the data which needs to be encoded
/// @returns an empty vector if there isnt enough space
/// @returns a vector of indices of edge pixels that can be used
/// @note this function should also be used for checking
///       the encoding, for the purposes of decoding
std::vector<uint32_t> edgeEncodable(const ImageData& grayscale, uint32_t length);

/// Encode secret bits into edge pixels of an image, this
/// is quite resistant to statistical steganalysis because
/// edge data has higher entropy so stored data looks less
/// suspicious from a mathematical perspective
/// @param img image wrapper class which contains host image data
/// @param possible_indices the indices returned by `edgeEncodable()`
/// @param data the data to write to the host image
/// @param password inputted password to shuffle indices
void encodeEdgeLSBMR(const Image& img, std::vector<uint32_t>& possible_indices,
                     const std::string& data, const std::string& password);

//================={ Header Item Separator }=================\\ 

/// Check whether or not an image has enough texture pixels
/// such that all of the secret bits can be encoded
/// @param img image wrapper class which contains host image data
/// @param data the data to write to the host image
/// @returns whether there is enough space
/// @note this function should also be used for checking
///       the encoding, for the purposes of decoding
bool textureEncodable(const ImageData& grayscale, uint32_t length);

/// Encode secret bits into texture pixels of an image, this
/// is quite resistant to statistical steganalysis because
/// textured data has higher entropy so stored data looks less
/// suspicious from a mathematical perspective
/// @param img image wrapper class which contains host image data
/// @param data the data to write to the host image
void encodeTextureLSBMR(const Image& img, const std::vector<uint32_t>& indices,
                        const std::string& data, const std::string& password);

}  // namespace khuzdoor::steg

#endif /* ENCODING_HPP */