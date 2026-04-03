// The Phi Project
// Copyright 2026 Khuzdoor Authors
// Use of this source code is governed by the GPL-3.0 license, found in the LICENSE file
//
// Date created: 28-03-2026
// Original author: Zevi Berlin (zeviberlin@gmail.com)
// Latest editor: Zevi Berlin (zeviberlin@gmail.com)

#include "steganography/Image.hpp"

#include <string>
#include <vector>
#include <cstdint>

#include "stb_image.h"
#include "stb_image_write.h"

#include "encryption/random.hpp"

//---------> [ Config. Separator ] <---------\\ 

khuzdoor::steg::Image::Image(const std::string& path_) : path(path_) {
  // 0: requested # channels per pixel, 0 = default
  this->data = stbi_load(this->path.c_str(), &this->width, &this->height, &this->channels, 0);
}

//------------[ Func. Implementation Separator ]------------\\ 

khuzdoor::steg::Image::~Image() {
  this->path = "";

  if (this->data != NULL) stbi_image_free(this->data);

  this->width = 0;
  this->height = 0;
  this->channels = 0;
}

//=====[ Declaration Separator ]=====\\ 
//------------[ Func. Implementation Separator ]------------\\ 
//=====[ Declaration Separator ]=====\\ 

bool khuzdoor::steg::Image::save(const std::string& new_path) {
  size_t ext_start_idx = new_path.find_last_of('.');
  if (ext_start_idx == std::string::npos) return false;

  const std::string ext = new_path.substr(ext_start_idx);

  if (ext == "png") {
    stbi_write_png(new_path.c_str(), this->width, this->height, this->channels, this->data,
                   this->width * this->channels);
  } else if (ext == "bmp") {
    stbi_write_bmp(new_path.c_str(), this->width, this->height, this->channels, this->data);
  } else {
    return false;
  }

  return true;
}