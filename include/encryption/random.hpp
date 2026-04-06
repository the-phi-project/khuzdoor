// The Phi Project
// Copyright 2026 Khuzdoor Authors
// Use of this source code is governed by the GPL-3.0 license, found in the LICENSE file
//
// Date created: 21-03-2026
// Original author: Zevi Berlin (zeviberlin@gmail.com)
// Latest editor: Zevi Berlin (zeviberlin@gmail.com)

#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <vector>
#include <array>
#include <string>
#include <cstdint>
#include <algorithm>

#include <sodium.h>

//---------> [ Config. Separator ] <---------\\ 

namespace khuzdoor::encryption {

/// Given a password, generate a seed for a PRNG such
/// that with the seed the PRNG is deterministic
/// uses BLAKE2b under the hood with `crypto_generichash()`
/// @param data usually a password, the actual data with which to seed
/// @returns the actual unsigned char array that seeds the function
std::array<unsigned char, randombytes_SEEDBYTES> generateSeed(const std::string& data);

/// Generate a list of deterministic seeded random indices [0, limit)
/// at which a pixel can be LSBM encoded. First it generates a list of
/// indices between `0` and `limit - 1`, then Fisher-Yates shuffles it,
/// then selects the first `data_size * 8` of them and returns
/// @param data_size size of data that needs to be encoded IN BYTES
/// @param limit size of host image (or just `0xFFFFFFFF`)
/// @param password the seed for the deterministic random pool
/// @returns the vector of random sorted NON-COLLIDING indices
std::vector<uint32_t> seededRandomIndices(uint32_t minimum, uint32_t data_size, uint32_t limit,
                                          const std::string& password);

/// Given a list of numbers, shuffle them "randomly" but in a
/// deterministic manner based on a password-generated seed
/// @param numbers the list of numbers
/// @param password the password with which to generate the seed
/// @returns NOTHING! Edits the list of numbers in place
void deterministicShuffle(std::vector<uint32_t>& numbers, const std::string& password);

}  // namespace khuzdoor::encryption

#endif /* RANDOM_HPP */