// TODO: DONT ALLOW JPEG!
// The Phi Project
// Copyright 2026 Khuzdoor Authors
// Use of this source code is governed by the GPL-3.0 license, found in the LICENSE file
//
// Date created: 04-03-2026
// Original author: Zevi Berlin (zeviberlin@gmail.com)
// Latest editor: Zevi Berlin (zeviberlin@gmail.com)

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <span>

#include <nlohmann/json.hpp>

#include "Image.hpp"
#include "encryption/compression.hpp"
#include "encryption/aegis.hpp"
#include "encryption/key_derivation.hpp"
#include "cli/processor.hpp"
#include "utils/str_utils.hpp"

#include "steganography/lsbmr.hpp"
#include "encryption/random.hpp"

#include "steganography/encoding.hpp"
#include "steganography/decoding.hpp"
#include "steganography/sobel.hpp"

using json = nlohmann::json;

//---------> [ Config. Separator ] <---------\\ 

void writeFileContent(uint8_t* img, int channels, const std::string& content);
void printFileContent(uint8_t* img, int channels);

int main(int argc, char** argv) {
  std::string password = "Oobie Doobie Banoobie";
  /*std::string master_key = khuzdoor::encryption::extractMasterKey(
    password, static_cast<std::string>(khuzdoor::encryption::aegis256KeySalt));
  std::string aegis_key = khuzdoor::encryption::deriveAegis256Key(master_key);
  std::string aegis_nonce = khuzdoor::encryption::deriveAegis256Nonce(master_key);*/


  // clang-format off
  std::string data =
    R"beacon(This is Master Obi-Wan Kenobi. I regret to report that both our Jedi Order and the
  Republic have fallen, with the dark shadow of the Empire rising to take their place. This message
  is a warning and a reminder for any surviving Jedi: trust in the Force. Do not return to the
  Temple. That time has passed, and our future is uncertain. Avoid Coruscant. Avoid detection. Be
  secret... but be strong. We will each be challenged: our trust, our faith, our friendships. But we
  must persevere and, in time, I believe a new hope will emerge. May the Force be with you,
  always.)beacon";
  // clang-format on

  // std::string encrypted = khuzdoor::encryption::aegisEncrypt(data, aegis_key, aegis_nonce);
  // std::string compressed = khuzdoor::encryption::gzipCompress(data);

  khuzdoor::steg::Image img(argv[1]);
  if (!img.loaded()) {
    std::cout << tmc::red << "Failed to load image `" << argv[1] << "\n";
    return 1;
  }

  // khuzdoor::steg::encodeRandomLSBMR(img, data, password);
  //  uint8_t arr[1250 * 3];
  //  writeFileContent(arr, 3, data);
  // std::cout << "decoded: \n" << khuzdoor::steg::decodeRandomLSBMR(img, password);
  // std::cout << "num edge pixels in img: " << khuzdoor::steg::edgeEncodable(img, 0) << "\n";
  // uint8_t* grayscale = (uint8_t*)malloc((img.getWidth() * img.getHeight()) * sizeof(uint8_t));
  // for (uint32_t p = 0; p < img.getWidth() * img.getHeight(); p++) {
  //   uint8_t r = *img.at(p * img.getChannels());
  //   uint8_t g = *img.at(p * img.getChannels() + 1);
  //   uint8_t b = *img.at(p * img.getChannels() + 2);
  //   grayscale[p] = khuzdoor::steg::RGB2GS(r, g, b);
  //   // std::cout << "r: " << (int)r << " | g: " << (int)g << " | b: " << (int)b << "  ->  "
  //   //           << (int)khuzdoor::steg::RGB2GS(r, g, b) << "\n";
  // }

  // khuzdoor::steg::ImageData grayscale = khuzdoor::steg::RGB2GS_image(img);

  // std::vector<uint32_t> edges = khuzdoor::steg::edgeEncodable(grayscale, data.size());
  // if (edges.empty()) return 1;

  // khuzdoor::steg::encodeEdgeLSBMR(img, edges, data, password);
  khuzdoor::steg::encodeRandomLSBMR(img, data, password);

  img.save(argv[2]);

  return 0;
}

//
//
//

void writeFileContent(uint8_t* img, int channels, const std::string& content) {
  std::vector<std::bitset<8>> data_bytes(content.size());  // 8 bits per byte
  for (size_t i = 0; i < data_bytes.size(); i++) {
    for (int bit = 7; bit >= 0; bit--) {
      data_bytes[i][bit] = (content[i] & (1U << bit)) != 0;
    }
  }

  //

  uint32_t length = content.size();
  std::bitset<8> length_bytes[sizeof(uint32_t)];
  for (size_t i = 0; i < sizeof(uint32_t); i++) {
    length_bytes[i] = (uint8_t)((length >> ((sizeof(uint32_t) - i) * 8 - 8)) & 0xFF);
  }

  std::cout << "length: " << std::bitset<32>(length) << "\nlength bytes: [";
  for (std::bitset<8> bits : length_bytes)
    std::cout << bits << ", ";
  std::cout << "]\n";


  std::vector<uint32_t> indices =
    khuzdoor::encryption::seededRandomIndices(0, sizeof(uint32_t), 192, "test_password");
  int byte = 0, bit = 0;
  for (size_t i = 0; i < sizeof(uint32_t) * 8; i++) {
    khuzdoor::steg::writeLSBMR(img + (indices[i] * channels), length_bytes[byte][7 - bit]);
    std::cout << "wrote `" << length_bytes[byte][7 - bit] << "` to index `" << indices[i] << "\n";

    if (bit == 7) {
      bit = 0;
      byte++;
    } else {
      bit++;
    }
  }
}

void printFileContent(uint8_t* img, int channels) {
  uint32_t length = 0;

  for (size_t i = 0; i < sizeof(uint32_t) * 8; i++) {
    length <<= 1;
    if (khuzdoor::steg::readLSBMR(img + (i * channels))) length++;
  }

  std::cout << "read length: " << length << "\n";
}
