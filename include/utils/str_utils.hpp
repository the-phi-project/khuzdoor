// The Phi Project
// Copyright 2026 Khuzdoor Authors
// Use of this source code is governed by the GPL-3.0 license, found in the LICENSE file
//
// Date created: 05-03-2026
// Original author: Zevi Berlin (zeviberlin@gmail.com)
// Latest editor: Zevi Berlin (zeviberlin@gmail.com)

#ifndef STR_UTILS_HPP
#define STR_UTILS_HPP

#include <string>
#include <vector>

#include <sodium.h>

//---------> [ Config. Separator ] <---------\\ 

namespace khuzdoor::utils {

std::vector<std::string> splitStringRandomly(const std::string& str);

}


#endif /* STR_UTILS_HPP */