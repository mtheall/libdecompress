#pragma once

#include <sys/types.h>

enum {
  TYPE_RAW  = 0x00,
  TYPE_LZSS = 0x10,
  TYPE_LZ11 = 0x11,
  TYPE_HUFF = 0x20,
  TYPE_RLE  = 0x30,
};

void rawDecompress (const void *in, void *out, size_t size);
void lzssDecompress(const void *in, void *out, size_t size);
void lz11Decompress(const void *in, void *out, size_t size);
void rleDecompress (const void *in, void *out, size_t size);

// assume size is a multiple of 2 and out is 16-bit aligned
// if not then the user is crazy
void rawDecompressVram (const void *in, void *out, size_t size);
void lzssDecompressVram(const void *in, void *out, size_t size);
void lz11DecompressVram(const void *in, void *out, size_t size);
void huffDecompressVram(const void *in, void *out, size_t size, int unit);
void rleDecompressVram (const void *in, void *out, size_t size);

