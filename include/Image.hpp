// The Phi Project
// Copyright 2026 Khuzdoor Authors
// Use of this source code is governed by the GPL-3.0 license, found in the LICENSE file
//
// Date created: 28-03-2026
// Original author: Zevi Berlin (zeviberlin@gmail.com)
// Latest editor: Zevi Berlin (zeviberlin@gmail.com)

#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <string>
#include <vector>
#include <cstdint>

#include "stb_image.h"
#include "stb_image_write.h"

#include "encryption/random.hpp"

//---------> [ Config. Separator ] <---------\\ 

namespace khuzdoor::steg {

/// Wrapper class for a pixel array (`uint8_t*`) provided by STBI
class Image {
  private:
    std::string path;

    uint8_t* data;

    int width = 0;
    int height = 0;
    int channels = 0;

    //~~~~~~~< Misc. Separator >~~~~~~~\\ 

  public:
    /// Default constructor for automatically loading
    /// a file into the object via `stbi_load()`, as well
    /// as load all of the file's attributes
    /// @param path_ the path of the file to open and read *ASSUMED TO BE A VERIFIED IMAGE FILE*
    /// @param success the output parameter for whether the load was successful or not
    explicit Image(const std::string& path_);

    // no copying, unique resource
    Image(const Image&) = delete;
    Image& operator=(const Image&) = delete;
    Image(Image&&) = default;

    ~Image();

    //=====[ Declaration Separator ]=====\\ 

    // clang-format off

    bool loaded() const { return this->data != NULL; }

    /**/

    int getWidth() const { return this->width; }
    int getHeight() const { return this->height; }
    int getChannels() const { return this->channels; }

    // clang-format on

    /// Accesses pixel data at the specified index with bounds checking
    /// @param idx The linear index into the flattened pixel data array.
    /// @return Pointer to the data at the specified index, or nullptr if out of bounds.
    /// @note This assumes the caller has dealt with channel offset
    /// @note Time complexity: O(1). The bounds check is a single comparison operation,
    ///       which is negligible compared to the actual memory access. It's worth the cost
    ///       as it prevents buffer overflows and undefined behavior
    uint8_t* at(uint32_t idx) const {
      return (idx > (uint32_t)(this->width * this->height * this->channels)) ? nullptr
                                                                             : this->data + idx;
    }

    /// Take the (edited?) image and save the data
    /// to a new file path - the format of the image
    /// comes from the extension, so `.png` -> `PNG Image`
    /// @param new_path the path to which the image data should be saved
    /// @returns the success of the saving
    /// @note this function assumes that the inputted path
    ///       has an extension which is a valid image extension
    bool save(const std::string& new_path);
};

//================={ Header Item Separator }=================\\ 

// NOLINTBEGIN -- I fucking get it you don't like C

/// Wrapper struct for just raw data, rather than
/// have an entire class with helper funcs
struct ImageData {
    std::vector<uint8_t> data;
    int width;
    int height;
    int channels;

    //=====[ Declaration Separator ]=====\\ 

    ImageData(int width_, int height_, int channels_)
        : width(width_),
          height(height_),
          channels(channels_),
          data(std::vector<uint8_t>(width_ * height_ * channels_)) {
    }
};

// NOLINTEND

}  // namespace khuzdoor::steg

#endif /* IMAGE_HPP */