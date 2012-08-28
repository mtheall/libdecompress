#pragma once
#include <feos.h>

typedef struct {
  union {
    u32 buf;
    u8  *buf8;
    u16 *buf16;
    u32 *buf32;
  };
  u16 hword;
} emitter_t;

static inline void emitInit(emitter_t *e, u16 *buf) {
  e->buf16 = buf;
  e->hword = 0;
}

static inline void emit(emitter_t *e, u8 value) {
  if((e->buf ^= 1) & 1)
    e->hword = value;
  else
    *(e->buf16++) = (e->hword)|(value<<8);
}
