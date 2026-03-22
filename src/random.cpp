// The Phi Project
// Copyright 2026 Khuzdoor Authors
// Use of this source code is governed by the GPL-3.0 license, found in the LICENSE file
//
// Date created: 21-03-2026
// Original author: Zevi Berlin (zeviberlin@gmail.com)
// Latest editor: Zevi Berlin (zeviberlin@gmail.com)

#include "encryption/random.hpp"

#include <vector>
#include <array>
#include <string>
#include <cstdint>
#include <algorithm>

#include <sodium.h>

//---------> [ Config. Separator ] <---------\\ 

std::vector<uint32_t> khuzdoor::encryption::seededRandomIndices(uint32_t data_size, uint32_t limit,
                                                                const std::string& password) {
  uint32_t num_indices = data_size * 8;  // 8 bits for every byte
  if (num_indices > limit) return {};    // Need enough space for non-colliding indices

  // derive seed from password
  std::array<unsigned char, randombytes_SEEDBYTES> seed{};
  std::copy(password.begin(),
            password.begin() + (password.size() < seed.size() ? password.size() : seed.size()),
            seed.begin());

  // create array that contains all possible indices
  std::vector<uint32_t> pool(limit);
  for (uint32_t i = 0; i < limit; i++) {
    pool[i] = i;
  }

  // generate random data, `* 4` b/c 4 bytes per uint32_t
  std::vector<uint8_t> random_buf(num_indices * 4);
  randombytes_buf_deterministic(random_buf.data(), random_buf.size(), seed.data());

  // Fisher-Yates shuffle
  size_t buf_offset = 0;
  for (uint32_t i = 0; i < num_indices; i++) {
    uint32_t random_val = 0;
    std::memcpy(&random_val, random_buf.data() + buf_offset, sizeof(uint32_t));
    buf_offset += sizeof(uint32_t);

    uint32_t j = i + (random_val % (limit - i));  // dont ask gng

    std::swap(pool[i], pool[j]);
  }

  // return first num_indices elements (which are now shuffled)
  std::vector<uint32_t> indices(pool.begin(), pool.begin() + num_indices);
  std::sort(indices.begin(), indices.end(), std::less<uint32_t>());
  return indices;
}