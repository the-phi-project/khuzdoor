// The Phi Project
// Copyright 2026 Khuzdoor Authors
// Use of this source code is governed by the GPL-3.0 license, found in the LICENSE file
//
// Date created: 07-03-2026
// Original author: Zevi Berlin (zeviberlin@gmail.com)
// Latest editor: Zevi Berlin (zeviberlin@gmail.com)

#ifndef FILE_UTILS_HPP
#define FILE_UTILS_HPP

#include <string>
#include <cstdlib>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <map>

#include <pwd.h>
#include <unistd.h>

namespace fs = std::filesystem;

//---------> [ Config. Separator ] <---------\\ 

namespace khuzdoor::utils {

[[nodiscard]] inline size_t getFileSize(const std::string& filepath) {
  if (!fs::exists(filepath)) return 0;
  return fs::file_size(filepath);
}

//================={ Header Item Separator }=================\\ 

[[nodiscard]] static std::string expandFilePath(const std::string& path) {
  std::string result{path};

  if (path[0] == '~') {
    std::string home(std::getenv("HOME"));  // NOLINT -- "theoretically" thread safe (cppreference)
    std::string temp(getpwuid(getuid())->pw_dir);  // NOLINT ^

    if (!home.empty()) {
      result = home + path.substr(1);
    }
    if (!temp.empty()) {
      result = temp + path.substr(1);
    }
  }

  return result;
}

}  // namespace khuzdoor::utils

#endif /* FILE_UTILS_HPP */