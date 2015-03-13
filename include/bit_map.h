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
  //return (bits >> index) & 1;
  return bits & (1 << index);
}

void bit_set(uint32_t *bits, int index, bool value) {
  *bits = value ? *bits | (1 << index) : *bits & ~(1 << index);
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
void bitmap_init(Bitmap *map) {
  memset(map, 0, sizeof(Bitmap));
}

bool bitmap_get(Bitmap *map, int index) {
  assert(index > -1 && index < 256);
  BitmapEntry *entry = &map->entries[index / 32]; // hopefully this compiles to a shift
  return bit_get(entry->bits, index % 32); // hopefuly this compiles to a mask
}

void bitmap_set(Bitmap *map, int index, bool value) {
  assert(index > -1 && index < 256);
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
  assert(index > -1 && index < 256);
  int i = index / 32;
  BitmapEntry *entry = &map->entries[i];

  int shift_amount = 32 - index % 32;
  printf("index %% 32: %d\n", index % 32);
  printf("bits: %u\n", entry->bits);
  printf("shift_amount: %d\n", shift_amount);

  printf("bitmap_get_offset(%d)\n", index);
  printf("(&map->entries[i])->offset: %d\n", entry->offset);

  //printf("32 - index %% 32: %d\n", 32 - (index % 32));
  printf("bit_count: %d\n", bit_count(entry->bits));
  //printf("bit_count: %d\n", bit_count(entry->bits >> (33 - (index % 32))));
  return entry->offset + bit_count(entry->bits << shift_amount);
  //return entry->offset + bit_count(entry->bits & (0xFFFFFFFF << (32 - index % 32)));
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
