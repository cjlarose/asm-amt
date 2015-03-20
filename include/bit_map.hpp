#ifndef BIT_MAP_H
#define BIT_MAP_H

#include <stdint.h>
#include <cassert>
#include <iostream>

#define BITSET_ENTRIES 8

struct BitmapEntry {
  uint32_t bits;
  int offset;
};

/******************************************************************************
 * Bit twiddling utils
 *****************************************************************************/
bool bit_get(uint32_t bits, int index) {
  return bits & (1 << index);
}

uint32_t bit_set(uint32_t bits, int index, bool value) {
  return value ? bits | (1 << index) : bits & ~(1 << index);
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
class Bitmap {
  struct BitmapEntry entries[BITSET_ENTRIES];

  public:
    Bitmap() : entries{{0}} {}
    bool get(int) const;
    void set(int, bool);
    int get_offset(int);
    friend std::ostream& operator<<(std::ostream& os, const Bitmap& bm);
};

bool Bitmap::get(int index) const {
  assert(index > -1 && index < 256);
  BitmapEntry entry = entries[index / 32]; // hopefully this compiles to a shift
  return bit_get(entry.bits, index % 32); // hopefuly this compiles to a mask
}

void Bitmap::set(int index, bool value) {
  assert(index > -1 && index < 256);
  int i = index / 32;

  // update bits
  BitmapEntry *entry = &entries[i];
  entry->bits = bit_set(entry->bits, index % 32, value);

  // update memoized offsets
  for (++i; i < BITSET_ENTRIES; ++i) {
    entry = &entries[i];
    value ? entry->offset++ : entry->offset--;
  }
}

int Bitmap::get_offset(int index) {
  assert(index > -1 && index < 256);
  int i = index / 32;
  BitmapEntry entry = entries[i];

  if (index % 32 == 0) // avoid undefined behavior
    return 0;
  uint32_t shift_amount = 32 - index % 32;
  return entry.offset + bit_count(entry.bits << shift_amount);
}

std::ostream& operator<<(std::ostream& os, const Bitmap& bm) {
  int i, j, offset;
  char bits[40];
  std::fill(bits, bits + 39, ' ');
  bits[39] = '\0';

  os << "i | bits                                             | offset\n";
  os << "-------------------------------------------------------------\n";
  for (i = 0; i < BITSET_ENTRIES; ++i) {
    for (j = 0; j < 32; ++j) {
      offset = j / 4 + j;
      bits[offset] = bm.get(i * 32 + j) ? '1' : '0';
    }

    os << i << " | 0x" << std::hex << bm.entries[i].bits << " : " << bits << " | " << std::dec << bm.entries[i].offset << '\n';
  }
  os << '\n';
  return os;
}

#endif
