#include <feos.h>
#include <string.h>

void rawDecompress(const u8 *in, u8 *out, int size) {
  memcpy(out, in, size);
}
