// The Phi Project
// Copyright 2026 Khuzdoor Authors
// Use of this source code is governed by the GPL-3.0 license, found in the LICENSE file
//
// Date created: 04-03-2026
// Original author: Zevi Berlin (zeviberlin@gmail.com)
// Latest editor: Zevi Berlin (zeviberlin@gmail.com)

#include "cli/processor.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <span>
#include <stdexcept>

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

int khuzdoor::cli::writeDataToFile(const std::vector<std::string>& split,
                                   const std::vector<int>& indices, khuzdoor::cli::Data& data) {
  data.file.advise_sequential();  // we go page by page through the file so this is good

  json j;
  for (size_t i = 0; i < split.size(); i++) {
    j[i] = {indices[i], split[i].size()};
  }  // this creates an array like [[242, 44], [791, 80]] where a str of length 44 is @idx 242


  for (int i = 0; i < split.size(); i++) {
    try {
      data.file.writeRegion(split[i], indices[i]);
    } catch (const std::out_of_range& exc) {
      std::cout << tmc::red << exc.what() << "\n";
      return FILE_ERROR;
    }
  }

  // legend because it represents the legend for how to read the file and extract the hidden data
  std::string legend = j.dump();
  size_t old = data.file.size();
  data.file.resize(old + legend.size() + 1);

  try {
    legend = khuzdoor::encryption::gzipCompress(legend);
  } catch (const std::runtime_error& exc) {
    std::cout << tmc::red << "Failed to compress legend: " << exc.what() << "\n" << tmc::reset;
    return ENCRYPTION_ERROR;
  }

  legend = khuzdoor::encryption::aegisEncrypt(legend, data.aegis_key, data.aegis_nonce);

  try {
    data.file.writeRegion(legend, old);
  } catch (const std::out_of_range& exc) {
    std::cout << tmc::red << "Failed to write legend to file: " << exc.what()
              << "\n(Hidden data written)\n"
              << tmc::reset;
    return FILE_ERROR;
  }

  return SUCCESS;
}

//------------[ Func. Implementation Separator ]------------\\ 

int khuzdoor::cli::extractJSONFromFile(khuzdoor::cli::Data& data, json& op) {
  data.file.advise_random();  // we read the end of the file and then go back to the beginning

  // TODO: Decoding logic and whatnot

  size_t idx = data.file.size() - 2;

  std::byte* chr = data.file.byteAt(idx);  // subtract \EOF and \0
  if (*chr != std::byte{']'}) {
    std::cout << tmc::red << "Failed to extract data from the file `" << data.file.path
              << "` because of improper JSON legend formatting\n"
              << tmc::reset;
    return DECRYPTION_ERROR;
  }

  idx--;
  while (idx > (data.file.size() / 2)) {  // hard limit that JSON cannot be more than half the file
    if (*(data.file.byteAt(idx)) == std::byte{'['}) {
      if (*(data.file.byteAt(idx - 1)) == std::byte{'['}) {
        // therefore we have encountered the '[[' which means we've found our JSON
        idx--;
        break;
      }
    }

    idx--;
  }

  std::span<std::byte> read = data.file.readRegion(idx, data.file.size() - idx);
  std::string region(reinterpret_cast<const char*>(read.data()), read.size());

  // this will fail also if we never found '[[' so it all works out
  if (!json::accept(region)) {
    std::cout << tmc::red << "Failed to extract data from the file `" << data.file.path
              << "` because no valid JSON legend was present\n"
              << tmc::reset;
    return DECRYPTION_ERROR;
  }

  op = json::parse(region);
  return SUCCESS;
}

//------------[ Func. Implementation Separator ]------------\\ 

std::string khuzdoor::cli::readDataFromFile(khuzdoor::cli::Data& data, const json& j) {
  data.file.readRegion(0, 1);     // to reset it and load from the top
  data.file.advise_sequential();  // now that its at the beginning we can read it sequentially

  size_t size = 0;
  for (const auto& item : j) {
    size += item[1].get<int>();  // gets the length from the pair
  }

  std::string output;
  output.reserve(size);  // arguably more efficient to do 2 for loops for this

  for (const auto& element : j) {
    // remember that element is [index, length]
    std::span<std::byte> read = data.file.readRegion(element[0].get<int>(), element[1].get<int>());
    output += std::string(reinterpret_cast<const char*>(read.data()), read.size());
  }

  return output;
}

//------------[ Func. Implementation Separator ]------------\\ 

int khuzdoor::cli::createIndicesForSplit(const std::vector<std::string>& split,
                                         khuzdoor::cli::Data& data, std::vector<int>& op) {
  data.file.advise_sequential();  // go through file sequentially to figure out where to put things

  // MAGIC
  magic_t detector = magic_open(MAGIC_MIME_TYPE);  // return MIME string instead of description
  // MAGIC_DB_PATH is "~/.magic/magic.mgc" for developers by default
  if (magic_load(detector, khuzdoor::utils::expandFilePath(MAGIC_DB_PATH).data()) != 0) {
    std::cout << tmc::red << "Error loading libmagic database: " << magic_error(detector) << "\n"
              << tmc::reset;
    magic_close(detector);
    return FILE_ERROR;
  }

  const char* given = magic_file(detector, data.file.path.data());
  if (given == NULL) {
    std::cout << tmc::red << "Error opening and reading magic number from file `" << data.file.path
              << "`: " << magic_error(detector) << "\n"
              << tmc::reset;
    magic_close(detector);
    return FILE_ERROR;
  }

  const std::string mime = given;
  magic_close(detector);  // memory safety guys!
  //
}