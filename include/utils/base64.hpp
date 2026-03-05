// The Phi Project
// Copyright 2026 Khuzdoor Authors
// Use of this source code is governed by the GPL-3.0 license, found in the LICENSE file
//
// Date created: 04-03-2026
// Original author: Zevi Berlin (zeviberlin@gmail.com)
// Latest editor: Zevi Berlin (zeviberlin@gmail.com)

#ifndef BASE64_HPP
#define BASE64_HPP

#include <string>

#include <sodium.h>

//---------> [ Config. Separator ] <---------\\ 

namespace khuzdoor::utils {

// IGNORES \n
[[nodiscard]] std::string toBase64(const std::string& str);

//================={ Header Item Separator }=================\\ 

// IGNORES \n
[[nodiscard]] std::string fromBase64(const std::string& str);

}  // namespace khuzdoor::utils


#endif /* BASE64_HPP */