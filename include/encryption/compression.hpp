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
#include <stdexcept>

#include <zlib.h>

//---------> [ Config. Separator ] <---------\\ 

namespace khuzdoor::encryption {

// Uses GZIP level 3
std::string gzipCompress(const std::string& str);

//================={ Header Item Separator }=================\\ 

std::string gzipDecompress(const std::string& str);

}  // namespace khuzdoor::encryption


#endif /* COMPRESSION_HPP */