// The Phi Project
// Copyright 2026 Khuzdoor Authors
// Use of this source code is governed by the GPL-3.0 license, found in the LICENSE file
//
// Date created: 05-03-2026
// Original author: Zevi Berlin (zeviberlin@gmail.com)
// Latest editor: Zevi Berlin (zeviberlin@gmail.com)

#include "MappedFile.hpp"

#include <filesystem>
#include <fstream>
#include <string>
#include <stdexcept>
#include <span>
#include <cstdint>
#include <cstddef>
#include <vector>
#include <functional>
#include <algorithm>

#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

namespace fs = std::filesystem;

//---------> [ Config. Separator ] <---------\\ 

khuzdoor::file::MappedFile::MappedFile(const std::string& path_) : path(path_) {
  int flags = O_RDWR;
  this->fd = open(this->path.data(), flags);
  if (this->fd == -1) throw std::runtime_error("Failed to open file `" + this->path + "`");

  // stat buffer that the OS will populate with file metadata (size, permissions, etc.)
  struct stat sbuf{};
  if (fstat(this->fd, &sbuf) == -1) {
    throw std::runtime_error("Failed to load stat buffer file `" + this->path + "`");
  }

  this->size_ = static_cast<size_t>(sbuf.st_size);  // dynamic size, no hardcoding

  // request read and write access
  int prot = PROT_READ | PROT_WRITE;

  // let the OS map the entire file into virtual mem space, nullptr = it can decide where
  this->data = static_cast<std::byte*>(mmap(nullptr, this->size_, prot, MAP_SHARED, this->fd, 0));
  if (this->data == MAP_FAILED) {
    throw std::runtime_error("Failed to memory-map the file `" + this->path + "`");
  }
}

//------------[ Func. Implementation Separator ]------------\\ 

void khuzdoor::file::MappedFile::writeRegion(const std::string& new_data, size_t offset) {
  if (offset > size_ || offset + new_data.size() > size_) {
    throw std::out_of_range("Failed to write to file `" + this->path +
                            "` - requested write would overflow file");
  }

  std::span<std::byte> dest = {this->data + offset, new_data.size()};
  std::ranges::copy(std::as_bytes(std::span{new_data.data(), new_data.size()}), dest.begin());
  // MS_ASYNC means "schedule the write-back, don't block waiting for it".
  // Remove this line if you'd rather let the OS decide when to flush.
  // safe call because once the block is written its no longer needed
  msync(this->data + offset, new_data.size(), MS_ASYNC);
}

/*::::::::::::::::::::::::::::::::::::::*\
|*:::::::::[ Access Separator ]:::::::::*|
\*::::::::::::::::::::::::::::::::::::::*/