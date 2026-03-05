// The Phi Project
// Copyright 2026 Khuzdoor Authors
// Use of this source code is governed by the GPL-3.0 license, found in the LICENSE file
//
// Date created: 04-03-2026
// Original author: Zevi Berlin (zeviberlin@gmail.com)
// Latest editor: Zevi Berlin (zeviberlin@gmail.com)

#ifndef COMPRESSION_HPP
#define COMPRESSION_HPP

#include <string>

#include <zlib.h>

//---------> [ Config. Separator ] <---------\\ 

namespace khuzdoor::encryption {

// This function uses the std::string::append method
// on `op` so ensure that the output string `op` is
// empty when passed in
//
// Uses GZIP level 3
bool gzipCompress(const std::string& str, std::string& op);

//================={ Header Item Separator }=================\\ 

// This function uses the std::string::append method
// on `op` so ensure that the output string `op` is
// empty when passed in
bool gzipDecompress(const std::string& str, std::string& op);

}  // namespace khuzdoor::encryption


#endif /* COMPRESSION_HPP */