#include <stdbool.h>
#include <stdlib.h>

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
  *bits = value ? *bits | (1 << index) : *bits & ~(1 << index);
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
  BitmapEntry *map = calloc(8, sizeof(BitmapEntry));
  return map;
}

bool bitmap_get(Bitmap map, int index) {
  BitmapEntry *entry = &map[index >> 3]; // hopefully this compiles to a shift
  return bit_get(entry->bits, index % 8); // hopefuly this compiles to a mask
}

void bitmap_set(Bitmap map, int index, bool value) {
  BitmapEntry *entry = &map[index >> 3];
  bit_set(&entry->bits, index % 8, value);
}

int bitmap_get_offset(Bitmap map, int index) {
  int i, sum = 0;
  BitmapEntry *entry;
  for (i = 0; i < index >> 3; ++i) {
    entry = &map[i >> 3];
    sum += entry->offset;
  }
  sum += bit_count(entry->bits >> (index + 1));
  return sum;
}
