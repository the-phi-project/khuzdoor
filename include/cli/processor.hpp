// The Phi Project
// Copyright 2026 Khuzdoor Authors
// Use of this source code is governed by the GPL-3.0 license, found in the LICENSE file
//
// Date created: 04-03-2026
// Original author: Zevi Berlin (zeviberlin@gmail.com)
// Latest editor: Zevi Berlin (zeviberlin@gmail.com)

#ifndef PROCESSOR_HPP
#define PROCESSOR_HPP

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

using json = nlohmann::json;
namespace tmc = termcolor;

//---------> [ Config. Separator ] <---------\\ 

namespace khuzdoor::cli {

int writeDataToFile(const std::vector<std::string>& split, const std::vector<int>& indices,
                    khuzdoor::cli::Data& data);

//================={ Header Item Separator }=================\\ 

int extractJSONFromFile(khuzdoor::cli::Data& data, json& op);

//================={ Header Item Separator }=================\\ 

// this function just sort of assumes that the JSON
// will be in the form of {index, length} because its
// been AEGIS encrypted so it can't really be tampered with
//
// it also assumes that it wont fail because even if it reads
// bullshit bytes its still just reading bytes
std::string readDataFromFile(khuzdoor::cli::Data& data, const json& j);

//================={ Header Item Separator }=================\\ 

inline std::vector<std::string> createSplit(const std::string& message) {
  return khuzdoor::utils::splitStringRandomly(message);
}

//================={ Header Item Separator }=================\\ 

int createIndicesForSplit(const std::vector<std::string>& split, khuzdoor::cli::Data& data,
                          std::vector<int>& op);

}  // namespace khuzdoor::cli


#endif /* PROCESSOR_HPP */