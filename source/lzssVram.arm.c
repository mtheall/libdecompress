#include <feos.h>
#include "emit.h"

void lzssDecompressVram(const u8 *in, u8 *out, int size) {
  u8  flags = 0;
  u8  mask  = 0;
  u32 len;
  u32 disp;
  emitter_t e;

  // initialize emitter
  emitInit(&e, (u16*)out);

  while(size > 0) {
    if(mask == 0) {
      // read in the flags data
      // from bit 7 to bit 0:
      //     0: raw byte
      //     1: compressed block
      flags = *in++;
      mask  = 0x80;
    }

    if(flags & mask) { // compressed block
      // disp: displacement
      // len:  length
      len  = (((*in)&0xF0)>>4)+3;
      disp = ((*in++)&0x0F);
      disp = disp<<8 | (*in++);

      size -= len;

      // for len, copy data from the displacement
      // to the current buffer position
      while(len--)
        emit(&e, *(out++ - disp - 1));
    }
    else { // uncompressed block
      // copy a raw byte from the input to the output
      emit(&e, *in++);
      out++;
      size--;
    }

    mask >>= 1;
  }
}
