#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "amt.hpp"

TEST_CASE ("Strings with different prefixes can be inserted", "[trie]") {
  ArrayMappedTrie trie;

  trie.insert("a");
  REQUIRE( trie.contains("a") );

  trie.insert("q");
  REQUIRE( trie.contains("q") );

  trie.insert("r");
  REQUIRE( trie.contains("r") );
}

TEST_CASE ("Strings with common prefixes can be inserted", "[trie]") {
  ArrayMappedTrie trie;

  trie.insert("ag");
  REQUIRE ( trie.contains("ag") );

  trie.insert("ab");
  REQUIRE ( trie.contains("ab") );

  trie.insert("ad");
  REQUIRE ( trie.contains("ad") );

  trie.insert("ar");
  REQUIRE ( trie.contains("ar") );
}
