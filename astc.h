#pragma once

#include <astc_codec_internals.h>

enum class CompressionSpeed {
  kVeryFast = 0,
  kFast,
  kMedium,
  kThorough,
  kExhaustive,
};

void EncodeAstc(
    const void* pixels, int width, int height, uint32_t gl_format,
    uint32_t gl_type, uint8_t** out_data, size_t* out_size, int footprint_x = 4,
    int footprint_y = 4,
    CompressionSpeed compression_speed = CompressionSpeed::kExhaustive);
