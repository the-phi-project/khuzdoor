// The Phi Project
// Copyright 2026 Khuzdoor Authors
// Use of this source code is governed by the GPL-3.0 license, found in the LICENSE file
//
// Date created: 04-03-2026
// Original author: Zevi Berlin (zeviberlin@gmail.com)
// Latest editor: Zevi Berlin (zeviberlin@gmail.com)

#include "encryption/compression.hpp"

#include <string>

#include <zlib.h>

constexpr int BUF_SIZE = 16384;

//---------> [ Config. Separator ] <---------\\ 

bool khuzdoor::encryption::gzipCompress(const std::string& str, std::string& op) {
  z_stream zstr{};
  zstr.zalloc = Z_NULL;
  zstr.zfree = Z_NULL;
  zstr.opaque = Z_NULL;

  if (deflateInit2(&zstr, 3, Z_DEFLATED,
                   15 + 16,  // gzip
                   8, Z_DEFAULT_STRATEGY) != Z_OK) {
    op = "";
    return false;
  }

  zstr.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(str.data()));
  zstr.avail_in = static_cast<uInt>(str.size());

  std::string output;
  char buffer[BUF_SIZE];

  int ret;
  do {
    zstr.next_out = reinterpret_cast<Bytef*>(buffer);
    zstr.avail_out = sizeof(buffer);

    ret = deflate(&zstr, Z_FINISH);
    output.append(buffer, sizeof(buffer) - zstr.avail_out);
  } while (ret == Z_OK);

  deflateEnd(&zstr);

  if (ret != Z_STREAM_END) {
    op = "";
    return false;
  }

  op = output;
  return true;
}

//------------[ Func. Implementation Separator ]------------\\ 

bool khuzdoor::encryption::gzipDecompress(const std::string& str, std::string& op) {
  z_stream zstr{};
  zstr.zalloc = Z_NULL;
  zstr.zfree = Z_NULL;
  zstr.opaque = Z_NULL;

  if (inflateInit2(&zstr, 15 + 16) != Z_OK) {
    op = "";
    return false;
  }

  zstr.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(str.data()));
  zstr.avail_in = static_cast<uInt>(str.size());

  std::string output;
  char buffer[BUF_SIZE];

  int ret;
  do {
    zstr.next_out = reinterpret_cast<Bytef*>(buffer);
    zstr.avail_out = sizeof(buffer);

    ret = inflate(&zstr, 0);
    output.append(buffer, sizeof(buffer) - zstr.avail_out);
  } while (ret == Z_OK);

  inflateEnd(&zstr);

  if (ret != Z_STREAM_END) {
    op = "";
    return false;
  }

  op = output;
  return true;
}