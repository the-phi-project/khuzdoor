// The Phi Project
// Copyright 2026 Khuzdoor Authors
// Use of this source code is governed by the GPL-3.0 license, found in the LICENSE file
//
// Date created: 04-03-2026
// Original author: Zevi Berlin (zeviberlin@gmail.com)
// Latest editor: Zevi Berlin (zeviberlin@gmail.com)

#include "encryption/aegis.hpp"

#include <iostream>
#include <string>
#include <array>
#include <vector>

#include <sodium.h>

//---------> [ Config. Separator ] <---------\\ 

std::string khuzdoor::encryption::aegisGenKey() {
  std::string key(crypto_aead_aegis256_KEYBYTES, '\0');
  crypto_aead_aegis256_keygen(reinterpret_cast<unsigned char*>(key.data()));
  return key;
}

//------------[ Func. Implementation Separator ]------------\\ 

void khuzdoor::encryption::aegisEncrypt(const std::string& data, const std::string& aegis_key,
                                        std::string& op_ciphertext, std::string& op_nonce) {
  std::array<unsigned char, crypto_aead_aegis256_NPUBBYTES> nonce{};
  randombytes_buf(nonce.data(), nonce.size());

  std::vector<unsigned char> ciphertext(data.size() + crypto_aead_aegis256_ABYTES);
  crypto_aead_aegis256_encrypt(ciphertext.data(),  // ciphertext
                               nullptr,  // &ciphertext_len - unused b/c ciphertext stored in string
                               reinterpret_cast<const unsigned char*>(data.data()),  // message
                               data.size(),   // message length
                               nullptr,       // additional data
                               0,             // additional data length
                               NULL,          // secret nonce, idek why libsodium has this
                               nonce.data(),  // nonce
                               reinterpret_cast<const unsigned char*>(aegis_key.data())  // key
  );

  op_ciphertext = std::string(ciphertext.begin(), ciphertext.end());
  op_nonce = std::string(nonce.begin(), nonce.end());
}

//------------[ Func. Implementation Separator ]------------\\ 

bool khuzdoor::encryption::aegisDecrypt(const std::string& ciphertext,
                                        const std::string& aegis_nonce,
                                        const std::string& aegis_key, std::string& op_data) {
  unsigned long long msg_len = ciphertext.size() - crypto_aead_aegis256_ABYTES;
  std::vector<unsigned char> decrypted(msg_len);

  // only storing result instead of putting it directly in if statement for readability
  int scs = crypto_aead_aegis256_decrypt(
    decrypted.data(),                                            // message
    &msg_len,                                                    // message length
    NULL,                                                        // I genuinely have no idea
    reinterpret_cast<const unsigned char*>(ciphertext.data()),   // ciphertext
    ciphertext.size(),                                           // ciphertext length
    nullptr,                                                     // additional data
    0,                                                           // additional data length
    reinterpret_cast<const unsigned char*>(aegis_nonce.data()),  // nonce
    reinterpret_cast<const unsigned char*>(aegis_key.data())     // key
  );

  if (scs == 0) {
    op_data = std::string(decrypted.begin(), decrypted.end());
    return true;
  }

  op_data = "";
  return false;
}