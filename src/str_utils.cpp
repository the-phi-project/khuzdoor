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
    size_t chunk_size = randombytes_uniform(100) + 1;

    output.push_back(str.substr(last_index, chunk_size));  // ITS A LENGTH NOT AN INDEX!

    last_index += chunk_size;
  }

  return output;
}