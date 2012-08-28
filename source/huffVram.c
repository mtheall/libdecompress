#include <feos.h>
#include <string.h>
#include "emit.h"

void huffDecompressVram(const u8 *in, u8 *out, int size, int unit) {
  u32 treeSize = ((*in)+1)*2; // size of the huffman header
  u32 word = 0;               // 32-bits of input bitstream
  u32 mask = 0;               // which bit we are reading
  u32 dataMask = (1<<unit)-1; // mask to apply to data
  const u8 *node;             // node in the huffman tree
  const u8 *child;            // child of a node
  const u8 *root;             // the root node of the huffman tree
  u32 offset;                 // offset from node to child
  emitter_t e;

  // initialize emitter
  emitInit(&e, (u16*)out);

  // point to the root of the huffman tree
  root = in+1;
  node = root;

  // move input pointer to beginning of bitstream
  in += treeSize;

  while(size > 0) {
    if(mask == 0) { // we exhausted 32 bits
      // reset the mask
      mask = 0x80000000;

      // read the next 32 bits
      memcpy(&word, in, 4);
      in += 4;
    }

    // read the current node's offset value
    offset = (*node)&0x1F;

    child = (u8*)(((u32)node)&(~1))+offset*2;

    if(word & mask) { // we read a 1
      // point to the "right" child
      child += 3;

      if((*node)&0x40) { // "right" child is a data node
        // copy the child node into the output buffer and apply mask
        emit(&e, (*child)&dataMask);
        size--;

        // start over at the root node
        node = root;
      }
      else // traverse to the "right" child
        node = child;
    }

    else { // we read a 0
      // point to the "left" child
      child += 2;

      if((*node)&0x80) { // "left" child is a data node
        // copy the child node into the output buffer and apply mask
        emit(&e, (*child)&dataMask);
        size--;

        // start over at the root node
        node = root;
      }
      else // traverse to the "left" child
        node = child;
    }

    // shift to read next bit (read bit 31 to bit 0)
    mask >>= 1;
  }
}
