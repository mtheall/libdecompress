#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
  LZ77,
  LZ77Vram,
  LZ11,
  LZ11Vram,
  HUFF,
  RAW,
  RAWVram,
  RLE,
  RLEVram,
} DecompressType;

void decompress(const void* src, void* dst, DecompressType type);
void decompressAuto(const void* src, void* dst, bool vramSafe);

#ifdef __cplusplus
}
#endif
