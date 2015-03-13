#include <stdio.h>
#include <assert.h>
#include "bit_map.h"
#include "amt.h"

int main() {
  int i;

  printf("Testing bitcount\n");

  assert(bit_count(0) == 0);
  assert(bit_count(1) == 1);
  assert(bit_count(8) == 1);
  assert(bit_count(15) == 4);
  assert(bit_count(16) == 1);

  printf("Testing bitmap get & set\n");
  Bitmap map;
  bitmap_init(&map);

  // test that everything is initialized to 0
  for (i = 0; i < 256; ++i)
    assert(!bitmap_get(&map, i));

  // set and check
  bitmap_set(&map, 217, true);

  bitmap_print(&map);

  for (i = 0; i < 256; ++i) {
    //printf("i: %d\n", i);
    assert(bitmap_get(&map, i) == (i == 217));
  }

  // check offset
  assert(bitmap_get_offset(&map, 0) == 0);
  assert(bitmap_get_offset(&map, 217) == 0);
  assert(bitmap_get_offset(&map, 218) == 1);
  assert(bitmap_get_offset(&map, 219) == 1);
  assert(bitmap_get_offset(&map, 253) == 1);
  assert(bitmap_get_offset(&map, 255) == 1);

  // unset and check
  bitmap_set(&map, 217, false);
  for (i = 0; i < 256; ++i)
    assert(!bitmap_get(&map, i));

  bitmap_print(&map);

  assert(bitmap_get_offset(&map, 218) == 0);

  // add lots of values!
  bitmap_init(&map);
  bitmap_print(&map);
  bitmap_set(&map, 0, true);
  bitmap_print(&map);
  bitmap_set(&map, 18, true);
  bitmap_print(&map);
  bitmap_set(&map, 43, true);
  bitmap_print(&map);
  bitmap_set(&map, 111, true);
  bitmap_print(&map);
  bitmap_set(&map, 176, true);
  bitmap_print(&map);
  bitmap_set(&map, 255, true);
  bitmap_print(&map);

  assert(bitmap_get(&map, 0));
  assert(bitmap_get(&map, 18));
  assert(bitmap_get(&map, 43));
  assert(bitmap_get(&map, 111));
  assert(bitmap_get(&map, 176));
  assert(bitmap_get(&map, 255));

  printf("Testing amt\n");
  BitMappedNode trie;
  amt_init(&trie);

  const char *hello = "hello";
  amt_insert(&trie, hello, 6);

  return 0;
}
