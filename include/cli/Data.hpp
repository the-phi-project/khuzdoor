// The Phi Project
// Copyright 2026 Khuzdoor Authors
// Use of this source code is governed by the GPL-3.0 license, found in the LICENSE file
//
// Date created: 06-03-2026
// Original author: Zevi Berlin (zeviberlin@gmail.com)
// Latest editor: Zevi Berlin (zeviberlin@gmail.com)

#ifndef DATA_HPP
#define DATA_HPP

#include <string>

#include "MappedFile.hpp"
#include "encryption/compression.hpp"
#include "encryption/key_derivation.hpp"
#include "encryption/aegis.hpp"

//---------> [ Config. Separator ] <---------\\ 

namespace khuzdoor::cli {

struct Data {
    khuzdoor::file::MappedFile& file;
    std::string aegis_key;
    std::string aegis_nonce;
};

}  // namespace khuzdoor::cli

#endif /* DATA_HPP */