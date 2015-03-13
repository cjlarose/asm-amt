#include <stdbool.h>
#include <stdlib.h>

typedef struct BitmapEntry {
  int bits;
  int offset;
} BitmapEntry;

typedef BitmapEntry* Bitmap;

// bit twiddling utils
bool bit_get(int bits, int index) {
  return (bits >> index) & 1;
}

void bit_set(int *bits, int index, bool value) {
  if (value)
    *bits = *bits | (1 << index);
  else
    *bits = *bits & ~(1 << index);
}

// bitmap functions
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
