// The Phi Project
// Copyright 2026 Khuzdoor Authors
// Use of this source code is governed by the GPL-3.0 license, found in the LICENSE file
//
// Date created: 05-03-2026
// Original author: Zevi Berlin (zeviberlin@gmail.com)
// Latest editor: Zevi Berlin (zeviberlin@gmail.com)

#include "encryption/key_derivation.hpp"

#include <string>
#include <string_view>
#include <array>

#include <sodium.h>

//---------> [ Config. Separator ] <---------\\ 

std::string khuzdoor::encryption::extractMasterKey(const std::string& str,
                                                   const std::string& salt) {
  // clang will optimize away the repeated operations
  std::string first_third = str.substr(0, (int)(str.size() / 3));
  std::string middle_third = str.substr((int)(str.size() / 3), (int)((str.size() * 2) / 3));
  std::string final_third = str.substr((int)((str.size() * 2) / 3));

  crypto_kdf_hkdf_sha512_state state{};

  crypto_kdf_hkdf_sha512_extract_init(&state, reinterpret_cast<const unsigned char*>(salt.data()),
                                      salt.size());

  crypto_kdf_hkdf_sha512_extract_update(
    &state, reinterpret_cast<const unsigned char*>(first_third.data()), first_third.size() - 1);
  crypto_kdf_hkdf_sha512_extract_update(
    &state, reinterpret_cast<const unsigned char*>(middle_third.data()), middle_third.size() - 1);
  crypto_kdf_hkdf_sha512_extract_update(
    &state, reinterpret_cast<const unsigned char*>(final_third.data()), final_third.size() - 1);

  std::array<unsigned char, crypto_kdf_hkdf_sha512_KEYBYTES> master{};
  crypto_kdf_hkdf_sha512_extract_final(&state, master.data());

  return std::string(master.begin(), master.end());
}

//------------[ Func. Implementation Separator ]------------\\

std::string khuzdoor::encryption::extractSizedData(int size, const std::string& master_key,
                                                   const std::string_view& context) {
  //
  std::string key(size, '\0');

  crypto_kdf_hkdf_sha512_expand(reinterpret_cast<unsigned char*>(key.data()), size, context.data(),
                                context.size() - 1,
                                reinterpret_cast<const unsigned char*>(master_key.data()));

  return key;
}