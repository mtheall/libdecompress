#include <feos.h>
#include "emit.h"

void rawDecompressVram(const u8 *in, u8 *out, int size) {
  u32 word;
  emitter_t e;
  
  // initialize emitter
  emitInit(&e, (u16*)out);

  if(size >= 2 && (e.buf & 2)) { // align to word
    size -= 2;
    word  = *in++;
    word |= (*in++)<<8;
    *(e.buf16++) = word;
  }
  while((size - 4) >= 0) { // write as many words as possible
    size -= 4;
    word  = *in++;
    word |= (*in++)<<8;
    word |= (*in++)<<16;
    word |= (*in++)<<24;
  }
  if((size - 2) >= 0) { // write a remaining halfword
    size -= 2;
    word  = *in++;
    word |= (*in++)<<8;
    *(e.buf16++) = word;
  }
}
