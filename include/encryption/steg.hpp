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

#include "cli/Data.hpp"
#include "MappedFile.hpp"

//---------> [ Config. Separator ] <---------\\ 

namespace khuzdoor::encryption {

// convert a std::byte into a std::bitset
inline std::bitset<8> toBits(std::byte byte) {
  std::bitset<8> set{};

  // https://stackoverflow.com/a/37417501
  for (int bit = 0; bit < 8; bit++) {
    set[bit] = (((uint8_t)byte >> bit) & 1) != 0;
  }

  return set;
}

//================={ Header Item Separator }=================\\ 

}  // namespace khuzdoor::encryption

#endif /* STEG_HPP */