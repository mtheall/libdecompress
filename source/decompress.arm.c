#include <feos.h>
#include <string.h>
#include "decompress.h"
#include "internal.h"

FEOS_EXPORT void decompress(const void* src, void* dst, DecompressType type) {
  u32 size;
  u8  header;
  bool vramSafe = ((u32)dst & 1) == 0;

  if(src == NULL || dst == NULL) // can't point to nothing
    return;

  header = *(u8*)src;
  memcpy(&size, src, 4);
  size >>= 8;
  src  += 4;

  switch(header & 0xF0) {
    case TYPE_RAW:
      if(type == RAW)
        rawDecompress(src, dst, size);
      else if(type == RAWVram && vramSafe)
        rawDecompressVram(src, dst, size);
      break;
    case TYPE_LZSS:
      if(header == TYPE_LZ11) {
        if(type == LZ11)
          lz11Decompress(src, dst, size);
        else if(type == LZ11Vram && vramSafe)
          lz11DecompressVram(src, dst, size);
      }
      else if(header == TYPE_LZSS) {
        if(type == LZ77)
          lzssDecompress(src, dst, size);
        else if(type == LZ77Vram && vramSafe)
          lzssDecompressVram(src, dst, size);
      }
      break;
    case TYPE_HUFF:
      if(vramSafe)
        huffDecompressVram(src, dst, size, header & 0x0F);
      break;
    case TYPE_RLE:
      if(type == RLE)
        rleDecompress(src, dst, size);
      else if(type == RLEVram && vramSafe)
        rleDecompressVram(src, dst, size);
      break;
  }
}

FEOS_EXPORT void decompressAuto(const void* src, void* dst, bool vramSafe) {
  u32 size;
  u8  header;

  if(src == NULL || dst == NULL) // can't point to nothing
    return;

  header = *(u8*)src;
  memcpy(&size, src, 4);
  size >>= 8;
  src  += 4;

  switch(header & 0xF0) {
    case TYPE_RAW:
      if(!vramSafe)
        rawDecompress(src, dst, size);
      else
        rawDecompressVram(src, dst, size);
      break;
    case TYPE_LZSS:
      if(header == TYPE_LZ11) {
        if(!vramSafe)
          lz11Decompress(src, dst, size);
        else
          lz11DecompressVram(src, dst, size);
      }
      else if(header == TYPE_LZSS) {
        if(!vramSafe)
          lzssDecompress(src, dst, size);
        else
          lzssDecompressVram(src, dst, size);
      }
      break;
    case TYPE_HUFF:
      huffDecompressVram(src, dst, size, header & 0x0F);
      break;
    case TYPE_RLE:
      if(!vramSafe)
        rleDecompress(src, dst, size);
      else
        rleDecompressVram(src, dst, size);
      break;
  }
}
