#include <stdio.h>
#include <assert.h>
#include "bit_map.h"

int main() {
  int i;

  printf("Testing bitcount\n");

  assert(bit_count(0) == 0);
  assert(bit_count(1) == 1);
  assert(bit_count(8) == 1);
  assert(bit_count(15) == 4);
  assert(bit_count(16) == 1);

  printf("Testing bitmap get & set\n");
  Bitmap map = bitmap_create();

  // test that everything is initialized to 0
  for (i = 0; i < 256; ++i)
    assert(!bitmap_get(map, i));

  // set and check
  bitmap_set(map, 217, true);

  bitmap_print(map);

  for (i = 0; i < 256; ++i) {
    //printf("i: %d\n", i);
    assert(bitmap_get(map, i) == (i == 217));
  }

  // check offset
  assert(bitmap_get_offset(map, 0) == 0);
  assert(bitmap_get_offset(map, 217) == 0);
  assert(bitmap_get_offset(map, 218) == 1);
  assert(bitmap_get_offset(map, 219) == 1);
  assert(bitmap_get_offset(map, 253) == 1);
  assert(bitmap_get_offset(map, 255) == 1);

  // unset and check
  bitmap_set(map, 217, false);
  for (i = 0; i < 256; ++i)
    assert(!bitmap_get(map, i));

  bitmap_print(map);

  assert(bitmap_get_offset(map, 218) == 0);

  return 0;
}
