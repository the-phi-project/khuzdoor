// The Phi Project
// Copyright 2026 Khuzdoor Authors
// Use of this source code is governed by the GPL-3.0 license, found in the LICENSE file
//
// Date created: 05-03-2026
// Original author: Zevi Berlin (zeviberlin@gmail.com)
// Latest editor: Zevi Berlin (zeviberlin@gmail.com)

#ifndef KEY_DERIVATION_HPP
#define KEY_DERIVATION_HPP

#include <string>
#include <string_view>
#include <array>

#include <sodium.h>

//---------> [ Config. Separator ] <---------\\ 

namespace khuzdoor::encryption {

// clang-format off
constexpr std::string_view aegis256KeySalt =
  "98ac62c247cff8c120d7199a7c6f957266b2718124f20a5631af27e67488f934112b35e2fe1730b5a729d71adc10c0200cce4d7989885fa56c2c103b4741ff18";
//constexpr std::string_view aegis256NonceSalt =
//  "9532ee00d86e36e8696553812b2ed67214c452b840ad496f72bd16e238dc5d029b346c80a48724584e8006c5409f2acaf8119949878a0fd83fb5ac35783aaa52";
// clang-format on

//=====[ Declaration Separator ]=====\\ 

std::string extractMasterKey(const std::string& str, const std::string& salt);

//================={ Header Item Separator }=================\\ 

std::string extractSizedData(int size, const std::string& master_key,
                             const std::string_view& context);

//================={ Header Item Separator }=================\\ 

inline std::string deriveAegis256Key(const std::string& master_key) {
  return extractSizedData(crypto_aead_aegis256_KEYBYTES, master_key, "aegis256key");
}

inline std::string deriveAegis256Nonce(const std::string& master_key) {
  return extractSizedData(crypto_aead_aegis256_NPUBBYTES, master_key, "aegis256nonce");
}

}  // namespace khuzdoor::encryption

#endif /* KEY_DERIVATION_HPP */