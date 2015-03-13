#include <stdbool.h>
#include <stdlib.h>

#define BITSET_ENTRIES 8

typedef struct BitmapEntry {
  unsigned int bits;
  int offset;
} BitmapEntry;

typedef BitmapEntry* Bitmap;

/******************************************************************************
 * Bit twiddling utils
 *****************************************************************************/
bool bit_get(unsigned int bits, int index) {
  return (bits >> index) & 1;
}

void bit_set(unsigned int *bits, int index, bool value) {
  *bits = value ? *bits | (1 << (32 - index)) : *bits & ~(1 << (32 - index));
}

int bit_count(unsigned int bits) {
  int count;
  for (count = 0; bits; bits >>= 1)
    count += bits & 1;
  return count;
}

/******************************************************************************
 * Bitmap functions
 *****************************************************************************/
Bitmap bitmap_create() {
  // create a bitmap of size 256 bits
  BitmapEntry *map = calloc(BITSET_ENTRIES, sizeof(BitmapEntry));
  return map;
}

bool bitmap_get(Bitmap map, int index) {
  BitmapEntry *entry = &map[index / 32]; // hopefully this compiles to a shift
  return bit_get(entry->bits, 32 - index % 32); // hopefuly this compiles to a mask
}

void bitmap_set(Bitmap map, int index, bool value) {
  int i = index / 32;

  // update bits
  BitmapEntry *entry = &map[i];
  bit_set(&entry->bits, index % 32, value);

  // update memoized offsets
  for (++i; i < BITSET_ENTRIES; ++i) {
    entry = &map[i];
    entry->offset++;
  }
}

int bitmap_get_offset(Bitmap map, int index) {
  int i = index / 32;
  return (&map[i])->offset + bit_count((&map[i])->bits >> (33 - index % 32));
}
