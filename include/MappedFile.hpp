// The Phi Project
// Copyright 2026 Khuzdoor Authors
// Use of this source code is governed by the GPL-3.0 license, found in the LICENSE file
//
// Date created: 05-03-2026
// Original author: Zevi Berlin (zeviberlin@gmail.com)
// Latest editor: Zevi Berlin (zeviberlin@gmail.com)

#ifndef MAPPEDFILE_HPP
#define MAPPEDFILE_HPP

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

namespace khuzdoor::file {

inline size_t getFileSize(const std::string& filepath) {
  if (!fs::exists(filepath)) return 0;
  return fs::file_size(filepath);
}

//================={ Header Item Separator }=================\\ 

// The purpose of this class is such that if the user chooses to
// encode data into a very large file (>2GB), which is sort of
// the actual primary usecase of the program, we can efficiently
// load the file into memory without taking up more than ~5% of
// memory with a huge file. Instead, this class takes the approach
// of mapping the file into memory and letting the operating system
// page the file from disk on demand and evict unused or finished
// pages, that way we don't have to do the heavy lifting and we can
// take control of the user's memory efficiently
class MappedFile {
  private:
    int fd = -1;

    std::byte* data = nullptr;
    size_t size_ = 0;

    std::string path;

  public:
    explicit MappedFile(const std::string& path);
    // no copying, unique resource
    MappedFile(const MappedFile&) = delete;
    MappedFile& operator=(const MappedFile&) = delete;
    MappedFile(MappedFile&&) = default;

    ~MappedFile() {
      if (this->data != nullptr) munmap(this->data, this->size_);
      if (this->fd != -1) close(this->fd);
    }

    //=====[ Declaration Separator ]=====\\ 

    std::span<const std::byte> view() const {
      return {this->data, this->size_};
    }

    std::span<std::byte> view() {
      return {this->data, this->size_};
    }

    size_t size() const {
      return this->size_;
    }

    //=====[ Declaration Separator ]=====\\ 

    std::span<std::byte> readRegion(size_t offset, size_t len) const {
      return {this->data + offset, len};
    }

    void writeRegion(const std::string& new_data, size_t offset);

    void resize(size_t new_size);

    //=====[ Declaration Separator ]=====\\ 

    // tell the kernel we plan to walk through the mapping from start to finish.
    // this lets it aggressively read-ahead pages before we access them,
    // reducing the number of page faults and disk stalls we experience
    // opposite reading pattern from `advise_random()`
    void advise_sequential() {
      madvise(this->data, this->size_, MADV_SEQUENTIAL);
    }

    // tell the kernel our access pattern is unpredictable (random offsets).
    // this disables read-ahead — prefetching would waste I/O bandwidth
    // since we won't necessarily access the pages that were prefetched
    // opposite reading pattern from `advise_sequential()`
    void advise_random() {
      madvise(this->data, this->size_, MADV_RANDOM);
    }
};

}  // namespace khuzdoor::file

#endif /* MAPPEDFILE_HPP */