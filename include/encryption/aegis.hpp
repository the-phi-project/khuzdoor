// The Phi Project
// Copyright 2026 Khuzdoor Authors
// Use of this source code is governed by the GPL-3.0 license, found in the LICENSE file
//
// Date created: 04-03-2026
// Original author: Zevi Berlin (zeviberlin@gmail.com)
// Latest editor: Zevi Berlin (zeviberlin@gmail.com)

#ifndef AEGIS_HPP
#define AEGIS_HPP

#include <string>
#include <array>
#include <vector>

#include <sodium.h>

//---------> [ Config. Separator ] <---------\\ 

namespace khuzdoor::encryption {

constexpr int AEGIS_KEY_SIZE = 32;  // 256 bit

//=====[ Declaration Separator ]=====\\ 

std::string aegisGenKey();

//================={ Header Item Separator }=================\\ 

// if nonce is left blank, one will be generated and put into
// the string reference
// returns the ciphertext
std::string aegisEncrypt(const std::string& data, const std::string& aegis_key,
                         std::string& aegis_nonce);

//================={ Header Item Separator }=================\\ 

bool aegisDecrypt(const std::string& ciphertext, const std::string& aegis_nonce,
                  const std::string& aegis_key, std::string& op_data);

}  // namespace khuzdoor::encryption

#endif /* AEGIS_HPP */