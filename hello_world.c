#include <stdio.h>
#include <assert.h>
#include "bit_map.h"

int main() {
  int i;

  printf("Testing bitmap\n");
  Bitmap map = bitmap_create();

  // test that everything is initialized to 0
  for (i = 0; i < 256; ++i)
    assert(!bitmap_get(map, i));

  // set and check
  bitmap_set(map, 240, true);

  for (i = 0; i < 256; ++i)
    if (i != 240)
      assert(!bitmap_get(map, i));

  // unset and check
  bitmap_set(map, 240, false);
  for (i = 0; i < 256; ++i)
    assert(!bitmap_get(map, i));

  return 0;
}
