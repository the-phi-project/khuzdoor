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

#include <sodium.h>

//---------> [ Config. Separator ] <---------\\ 

namespace khuzdoor::encryption {

/// generate a list of deterministic seeded random indices [0, limit)
/// at which a pixel can be LSBM encoded. First it generates a list of
/// indices between `0` and `limit - 1`, then Fisher-Yates shuffles it, then
/// selects the first `data_size * 8` of them and returns
/// @param data_size size of data that needs to be encoded IN BYTES
/// @param limit size of host image (or just `0xFFFFFFFF`)
/// @param password the seed for the deterministic random pool
/// @returns the vector of random NON-COLLIDING indices
std::vector<uint32_t> seededRandomIndices(uint32_t data_size, uint32_t limit, const std::string& password);

}  // namespace khuzdoor::encryption

#endif /* RANDOM_HPP */