// The Phi Project
// Copyright 2026 Khuzdoor Authors
// Use of this source code is governed by the GPL-3.0 license, found in the LICENSE file
//
// Date created: 08-03-2026
// Original author: Zevi Berlin (zeviberlin@gmail.com)
// Latest editor: Zevi Berlin (zeviberlin@gmail.com)

#include "cli/processor.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <span>
#include <stdexcept>
#include <optional>

#include <nlohmann/json.hpp>
#include <termcolor/termcolor.hpp>
#include <cxxopts.hpp>
#include <magic.h>

#include "MappedFile.hpp"
#include "encryption/compression.hpp"
#include "encryption/key_derivation.hpp"
#include "encryption/aegis.hpp"
#include "cli/Data.hpp"
#include "utils/str_utils.hpp"
#include "utils/base64.hpp"
#include "utils/file_utils.hpp"

constexpr int SUCCESS = 0;
constexpr int USAGE_ERROR = 1;
constexpr int FILE_ERROR = 2;
constexpr int ENCRYPTION_ERROR = 3;
constexpr int DECRYPTION_ERROR = 4;

using json = nlohmann::json;
namespace tmc = termcolor;

//---------> [ Config. Separator ] <---------\\ 

std::optional<size_t> khuzdoor::cli::findIDATBlob(size_t start_idx, khuzdoor::cli::Data& data) {
  data.file.advise_sequential();  // we will be going through the file sequentially

  while (start_idx < data.file.size()) {
    std::span<std::byte> bytes = data.file.readRegion(start_idx, 4);
    if (bytes.size() < 4) break;

    // convert the big-endian bytes into an unsigned int
    uint32_t chunk_size =
      static_cast<uint32_t>((bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | (bytes[3]));

    std::span<std::byte> chunk_type = data.file.readRegion(start_idx + 4, 4);
    // check if the chunk is of type IDAT (image data chunk)
    if (chunk_type.size() == 4 &&           //
        chunk_type[0] == std::byte{'I'} &&  //
        chunk_type[1] == std::byte{'D'} &&  //
        chunk_type[2] == std::byte{'A'} &&  //
        chunk_type[3] == std::byte{'T'}) {
      return start_idx + 8;  // start of the IDAT region
    }

    // move - skip length (4) + type (4) + data (chunk_size) + CRC (4)
    start_idx += 12 + chunk_size;
  }

  return std::nullopt;  // no IDAT chunks found
}

//------------[ Func. Implementation Separator ]------------\\ 

std::optional<size_t> khuzdoor::cli::findSOSBlob(size_t start_idx, khuzdoor::cli::Data& data) {
  data.file.advise_sequential();  // we will be going through the file sequentially

  while (start_idx + 2 < data.file.size()) {
    std::span<std::byte> marker = data.file.readRegion(start_idx, 2);
    if (marker.size() < 2) break;

    // check for SOS marker 0xFFDA
    if (marker[0] == std::byte{0xFF} && marker[1] == std::byte{0xDA}) {
      std::span<std::byte> len_bytes = data.file.readRegion(start_idx + 2, 2);
      if (marker.size() < 2) break;

      // convert length bytes into an int
      uint16_t seg_len = static_cast<uint16_t>((len_bytes[0] << 8) | (len_bytes[1]));
      return start_idx + 2 + seg_len;  // Start of SOS data
    }

    start_idx++;  // I know this is a slow ahh loop
  }

  return std::nullopt;  // no SOS chunks found
}