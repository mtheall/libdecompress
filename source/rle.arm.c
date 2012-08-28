#include <feos.h>
#include <string.h>

void rleDecompress(const u8 *in, u8 *out, int size) {
  int len;
  u8  byte;

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
      memset(out, byte, len);
      out += len;
    }

    else { // uncompressed block
      // read the length of uncompressed bytes
      len = (byte & (0x7F)) + 1;
      size -= len;

      // for len, copy from input into output
      memcpy(out, in, len);
      out += len;
      in  += len;
    }
  }
}
