#include <feos.h>
#include <stdio.h>
#include "emit.h"

void rleDecompressVram(const u8 *in, u8 *out, int size) {
  int len;
  u32 word;
  u8  byte;
  emitter_t e;

  // initialize emitter
  emitInit(&e, (u16*)out);

  while(size > 0) {
    // read in the data header
    byte = *in++;

    if(byte & 0x80) { // compressed block
      // read the length of the run
      len = (byte & (0x7F)) + 3;
      size -= len;

      // read in the byte used for the run
      byte = *in++;

      // for len, copy byte into output
      if(e.buf & 1) { // align to halfword
        emit(&e, byte);
        len--;
      }
      if(e.buf & 2) { // align to word
        *(e.buf16++) = byte|(byte<<8);
        len -= 2;
      }
      while((len - 4) >= 0) { // write as many words as possible
        len -= 4;
        *(e.buf32++) = byte|(byte<<8)|(byte<<16)|(byte<<24);
      }
      if((len - 2) >= 0) { // write a remaining halfword
        *(e.buf16++) = byte|(byte<<8);
        len -= 2;
      }
      if(len) // write a remaining byte
        emit(&e, byte);
    }

    else { // uncompressed block
      // read the length of uncompressed bytes
      len = (byte & (0x7F)) + 1;
      size -= len;

      // for len, copy from input into output
      if(e.buf & 1) { // align to halfword
        emit(&e, *in++);
        len--;
      }
      if(len >= 2 && (e.buf & 2)) { // align to word
        len -= 2;
        word  = *in++;
        word |= (*in++)<<8;
        *(e.buf16++) = word;
      }
      while((len - 4) >= 0) { //write as many words as possible
        len -= 4;
        word  = *in++;
        word |= (*in++)<<8;
        word |= (*in++)<<16;
        word |= (*in++)<<24;
        *(e.buf32++) = word;
      }
      if((len - 2) >= 0) { // write a remaining halfword
        len -= 2;
        word  = *in++;
        word |= (*in++)<<8;
        *(e.buf16++) = word;
      }
      if(len) // write a remaining byte
        emit(&e, *in++);
    }
  }
}
