#include <stdio.h>
#include <assert.h>
#include "bit_map.hpp"
#include "amt.hpp"

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

  // test that everything is initialized to 0
  for (i = 0; i < 256; ++i)
    assert(!map.get(i));

  // set and check
  map.set(217, true);

  std::cout << map;

  for (i = 0; i < 256; ++i) {
    //printf("i: %d\n", i);
    assert(map.get(i) == (i == 217));
  }

  // check offset
  assert(map.get_offset(0) == 0);
  //printf("bitmap_get_offset(217): %d\n", map.get_offset(217));
  assert(map.get_offset(217) == 0);
  assert(map.get_offset(218) == 1);
  assert(map.get_offset(219) == 1);
  assert(map.get_offset(253) == 1);
  assert(map.get_offset(255) == 1);

  // unset and check
  map.set(217, false);
  for (i = 0; i < 256; ++i)
    assert(!map.get(i));

  std::cout << map;

  assert(map.get_offset(218) == 0);

  // add lots of values!
  std::cout << map;
  map.set(0, true);
  std::cout << map;
  map.set(18, true);
  std::cout << map;
  map.set(43, true);
  std::cout << map;
  map.set(111, true);
  std::cout << map;
  map.set(176, true);
  std::cout << map;
  map.set(255, true);
  std::cout << map;

  assert(map.get(0));
  assert(map.get(18));
  assert(map.get(43));
  assert(map.get(111));
  assert(map.get(176));
  assert(map.get(255));

  printf("Testing amt\n");
  ArrayMappedTrie trie;

  trie.insert("a", 2);
  assert(trie.contains("a", 2));

  trie.insert("ag", 3);
  assert(trie.contains("ag", 3));

  trie.insert("ab", 3);
  trie.contains("ab", 3);

  trie.insert("ad", 3);
  trie.contains("ad", 3);

  return 0;
}
