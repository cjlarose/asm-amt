#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "amt.hpp"

TEST_CASE ("Strings with different prefixes can be inserted", "[trie]") {
  Trie trie;

  trie.insert("a");
  REQUIRE( trie.contains("a") );

  trie.insert("q");
  REQUIRE( trie.contains("q") );

  trie.insert("r");
  REQUIRE( trie.contains("r") );
}

TEST_CASE ("Strings with common prefixes can be inserted", "[trie]") {
  Trie trie;

  trie.insert("ag");
  REQUIRE ( trie.contains("ag") );

  trie.insert("ab");
  REQUIRE ( trie.contains("ab") );

  trie.insert("ad");
  REQUIRE ( trie.contains("ad") );

  trie.insert("ar");
  REQUIRE ( trie.contains("ar") );
}

TEST_CASE ("Size begins at zero", "[trie]") {
  Trie trie;

  REQUIRE ( trie.size() == 0 );
}

TEST_CASE ("Size increases upon insertion", "[trie]") {
  Trie trie;

  trie.insert("a");
  REQUIRE ( trie.size() == 1 );

  trie.insert("b");
  REQUIRE ( trie.size() == 2 );

  trie.insert("c");
  REQUIRE ( trie.size() == 3 );

  trie.insert("ab");
  REQUIRE ( trie.size() == 4 );

  trie.insert("ad");
  REQUIRE ( trie.size() == 5 );

  trie.insert("add");
  REQUIRE ( trie.size() == 6 );
}
