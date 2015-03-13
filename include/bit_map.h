#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

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
    value ? entry->offset++ : entry->offset--;
  }
}

int bitmap_get_offset(Bitmap map, int index) {
  //printf("bitmap_get_offset(%d)\n", index);
  int i = index / 32;
  return (&map[i])->offset + bit_count((&map[i])->bits >> (33 - index % 32));
}

void bitmap_print(Bitmap map) {
  int i, j, offset;
  char bits[40];
  memset(bits, ' ', 39);
  bits[39] = '\0';

  printf("i | bits                                             | offset\n");
  printf("-------------------------------------------------------------\n");
  for (i = 0; i < BITSET_ENTRIES; ++i) {
    for (j = 0; j < 32; ++j) {
      offset = j / 4 + j;
      bits[offset] = bitmap_get(map, i * 32 + j) ? '1' : '0';
    }

    printf("%d | 0x%04x : %s | %d\n",
           i,
           (&map[i])->bits,
           bits,
           (&map[i])->offset);
  }
}
