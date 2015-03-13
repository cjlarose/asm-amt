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
  bitmap_set(map, 240, true);

  for (i = 0; i < 256; ++i)
    assert(bitmap_get(map, i) == (i == 240));

  // unset and check
  bitmap_set(map, 240, false);
  for (i = 0; i < 256; ++i)
    assert(!bitmap_get(map, i));

  return 0;
}
