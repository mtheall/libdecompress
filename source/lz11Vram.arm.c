#include <feos.h>
#include "emit.h"

void lz11DecompressVram(const u8 *in, u8 *out, int size) {
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
      switch((*in)>>4) {
        case 0: // extended block
          len   = (*in++)<<4;
          len  |= ((*in)>>4);
          len  += 0x11;
          break;
        case 1: // extra extended block
          len   = ((*in++)&0x0F)<<12;
          len  |= (*in++)<<4;
          len  |= ((*in)>>4);
          len  += 0x111;
          break;
        default: // normal block
          len   = ((*in)>>4)+1;
      }

      disp  = ((*in++)&0x0F)<<8;
      disp |= *in++;

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
