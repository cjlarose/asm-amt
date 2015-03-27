#include <stdio.h>
#include <assert.h>
#include "bit_map.hpp"
#include "amt.hpp"

int main() {
  int i;

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

  trie.insert("ar", 3);
  trie.contains("ar", 3);

  return 0;
}
