#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "amt.hpp"

TEST_CASE ("Strings with different prefixes can be inserted", "[trie]") {
  ArrayMappedTrie trie;

  trie.insert("a", 2);
  REQUIRE( trie.contains("a", 2) );

  trie.insert("q", 2);
  REQUIRE( trie.contains("q", 2) );

  trie.insert("r", 2);
  REQUIRE( trie.contains("r", 2) );
}

TEST_CASE ("Strings with common prefixes can be inserted", "[trie]") {
  ArrayMappedTrie trie;

  trie.insert("ag", 3);
  REQUIRE ( trie.contains("ag", 3) );

  trie.insert("ab", 3);
  REQUIRE ( trie.contains("ab", 3) );

  trie.insert("ad", 3);
  REQUIRE ( trie.contains("ad", 3) );

  trie.insert("ar", 3);
  REQUIRE ( trie.contains("ar", 3) );
}
