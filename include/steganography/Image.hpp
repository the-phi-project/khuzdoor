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

#include "stb_image.h"
#include "stb_image_write.h"

#include "encryption/random.hpp"

//---------> [ Config. Separator ] <---------\\ 

namespace khuzdoor::steg {

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

    /// Take the (edited?) image and save the data
    /// to a new file path - the format of the image
    /// comes from the extension, so `.png` -> `PNG Image`
    /// therefore its assumed that the output path has
    /// a verified safe format
    /// @param new_path the path to which the image data should be saved
    /// @returns the success of the saving
    bool save(const std::string& new_path);
};

}  // namespace khuzdoor::steg

#endif /* IMAGE_HPP */