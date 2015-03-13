#ifndef BIT_MAP_H
#define BIT_MAP_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define BITSET_ENTRIES 8

typedef struct BitmapEntry {
  uint32_t bits;
  int offset;
} BitmapEntry;

//typedef BitmapEntry Bitmap[8];
typedef struct Bitmap {
  BitmapEntry entries[BITSET_ENTRIES];
} Bitmap;

/******************************************************************************
 * Bit twiddling utils
 *****************************************************************************/
bool bit_get(uint32_t bits, int index) {
  return (bits >> index) & 1;
}

void bit_set(uint32_t *bits, int index, bool value) {
  *bits = value ? *bits | (1 << (32 - index)) : *bits & ~(1 << (32 - index));
}

int bit_count(uint32_t bits) {
  int count;
  for (count = 0; bits; bits >>= 1)
    count += bits & 1;
  return count;
}

/******************************************************************************
 * Bitmap functions
 *****************************************************************************/
Bitmap *bitmap_create() {
  // create a bitmap of size 256 bits
  Bitmap *map = (Bitmap *) calloc(1, sizeof(Bitmap));
  return map;
}

void bitmap_init(Bitmap *map) {
  memset(map, 0, sizeof(Bitmap));
}

bool bitmap_get(Bitmap *map, int index) {
  BitmapEntry *entry = &map->entries[index / 32]; // hopefully this compiles to a shift
  return bit_get(entry->bits, 32 - index % 32); // hopefuly this compiles to a mask
}

void bitmap_set(Bitmap *map, int index, bool value) {
  int i = index / 32;

  // update bits
  BitmapEntry *entry = &map->entries[i];
  bit_set(&entry->bits, index % 32, value);

  // update memoized offsets
  for (++i; i < BITSET_ENTRIES; ++i) {
    entry = &map->entries[i];
    value ? entry->offset++ : entry->offset--;
  }
}

int bitmap_get_offset(Bitmap *map, int index) {
  int i = index / 32;
  return (&map->entries[i])->offset + bit_count((&map->entries[i])->bits >> (33 - index % 32));
}

void bitmap_print(Bitmap* map) {
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

    printf("%d | 0x%08x : %s | %d\n",
           i,
           (&map->entries[i])->bits,
           bits,
           (&map->entries[i])->offset);
  }
  printf("\n");
}

#endif
