// The Phi Project
// Copyright 2026 Khuzdoor Authors
// Use of this source code is governed by the GPL-3.0 license, found in the LICENSE file
//
// Date created: 05-03-2026
// Original author: Zevi Berlin (zeviberlin@gmail.com)
// Latest editor: Zevi Berlin (zeviberlin@gmail.com)

#include "utils/str_utils.hpp"

#include <string>
#include <vector>

#include <sodium.h>

//---------> [ Config. Separator ] <---------\\ 

std::vector<std::string> khuzdoor::utils::splitStringRandomly(const std::string& str) {
  std::vector<std::string> output;

  size_t last_index = 0;
  while (last_index < str.size()) {
    size_t next_index = randombytes_uniform(str.size());
    if (next_index < last_index) next_index += last_index;

    std::string sub = str.substr(last_index, next_index);

    output.push_back(sub);

    last_index = next_index;
  }

  return output;
}