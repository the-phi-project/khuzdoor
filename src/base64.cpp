// The Phi Project
// Copyright 2026 Khuzdoor Authors
// Use of this source code is governed by the GPL-3.0 license, found in the LICENSE file
//
// Date created: 04-03-2026
// Original author: Zevi Berlin (zeviberlin@gmail.com)
// Latest editor: Zevi Berlin (zeviberlin@gmail.com)

#include "utils/base64.hpp"

#include <string>

#include <sodium.h>

//---------> [ Config. Separator ] <---------\\ 

[[nodiscard]] std::string khuzdoor::utils::toBase64(const std::string& str) {
  // libsodium's base64 encoded length (no newlines, standard variant)
  const size_t encodedLen = sodium_base64_encoded_len(str.size(), sodium_base64_VARIANT_ORIGINAL);

  std::string opt(encodedLen, '\0');
  sodium_bin2base64(opt.data(), encodedLen, reinterpret_cast<const unsigned char*>(str.data()),
                    str.size(), sodium_base64_VARIANT_ORIGINAL);

  if (!opt.empty() && opt.back() == '\0') {
    opt.pop_back();
  }

  return opt;
}

//------------[ Func. Implementation Separator ]------------\\ 

[[nodiscard]] std::string khuzdoor::utils::fromBase64(const std::string& str) {
  std::string opt(str.size(), '\0');
  size_t decodedLen = 0;

  if (sodium_base642bin(reinterpret_cast<unsigned char*>(opt.data()), opt.size(), str.data(),
                        str.size(),
                        nullptr,  // ignore_chars (none)
                        &decodedLen,
                        nullptr,  // b64_end pointer (unused)
                        sodium_base64_VARIANT_ORIGINAL) != 0) {
    return "";
  }

  opt.resize(decodedLen);
  return opt;
}