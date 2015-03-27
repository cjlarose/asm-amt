#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "bit_map.hpp"

TEST_CASE( "Bit values can be retrived", "[bit_utils]" ) {
  int i;
  for (i = 0; i < 32; i++)
    REQUIRE( !bit_get(0, i) );

  for (i = 0; i < 32; i++)
    REQUIRE( bit_get(~0, i) );

  REQUIRE( !bit_get(4, 0) );
  REQUIRE( !bit_get(4, 1) );
  REQUIRE( bit_get(4, 2) );
  REQUIRE( !bit_get(4, 3) );
  REQUIRE( !bit_get(4, 4) );
}

TEST_CASE ("Bit indicies can be turned on or off", "[bit_utils]") {
  REQUIRE( bit_set(0, 0, true) == 1 );
  REQUIRE( bit_set(0, 1, true) == 2 );
  REQUIRE( bit_set(0, 2, true) == 4 );
  REQUIRE( bit_set(0, 3, true) == 8 );
  REQUIRE( bit_set(0, 4, true) == 16 );
  REQUIRE( bit_set(0, 31, true) == 2147483648 );

  REQUIRE( bit_set(255, 0, true) == 255 );
  REQUIRE( bit_set(255, 0, false) == 254 );
  REQUIRE( bit_set(255, 1, false) == 255 - 2);
  REQUIRE( bit_set(255, 2, false) == 255 - 4 );
  REQUIRE( bit_set(255, 3, false) == 255 - 8 );
  REQUIRE( bit_set(255, 4, false) == 255 - 16 );
  REQUIRE( bit_set(255, 5, false) == 255 - 32 );
  REQUIRE( bit_set(255, 6, false) == 255 - 64 );
  REQUIRE( bit_set(255, 7, false) == 255 - 128 );
  REQUIRE( bit_set(255, 8, false) == 255 );
  REQUIRE( bit_set(255, 31, false) == 255 );
}

TEST_CASE ("Bits can be counted", "[bit_utils]") {
  REQUIRE( bit_count(0) == 0 );
  REQUIRE( bit_count(1) == 1 );
  REQUIRE( bit_count(8) == 1 );
  REQUIRE( bit_count(15) == 4 );
  REQUIRE( bit_count(16) == 1 );
  REQUIRE( bit_count(32) == 1 );
  REQUIRE( bit_count(255) == 8 );
  REQUIRE( bit_count(~0) == 32 );
}

TEST_CASE ("Bitmap starts off empty", "[bitmap]") {
  Bitmap map;
  int i;

  // test that everything is initialized to 0
  for (i = 0; i < 256; ++i)
    REQUIRE ( !map.get(i) );
}

TEST_CASE ("Setting single value in bitmap changes only one bit", "[bitmap]") {
  Bitmap map;
  int i;

  map.set(217, true);
  for (i = 0; i < 256; ++i) {
    REQUIRE ( map.get(i) == (i == 217) );
  }
}

TEST_CASE ("Unsetting single value in bitmap changes only one bit", "[bitmap]") {
  Bitmap map;
  int i;

  for (i = 0; i < 256; ++i)
    map.set(i, true);

  map.set(217, false);

  for (i = 0; i < 256; ++i)
    REQUIRE ( map.get(i) == (i != 217) );
}

TEST_CASE ("Offset returns number of 1 bits before the given index", "[bitmap]") {
  Bitmap map;

  map.set(217, true);

  // check offset
  REQUIRE ( map.get_offset(0) == 0 );
  REQUIRE ( map.get_offset(217) == 0 );
  REQUIRE ( map.get_offset(218) == 1 );
  REQUIRE ( map.get_offset(219) == 1 );
  REQUIRE ( map.get_offset(253) == 1 );
  REQUIRE ( map.get_offset(255) == 1 );

  map.set(217, false);
  REQUIRE ( map.get_offset(217) == 0 );
}

TEST_CASE ("Many bits can be set", "[bitmap]") {
  Bitmap map;

  map.set(0, true);
  map.set(18, true);
  map.set(43, true);
  map.set(111, true);
  map.set(176, true);
  map.set(255, true);

  REQUIRE ( map.get(0) );
  REQUIRE ( map.get(18) );
  REQUIRE ( map.get(43) );
  REQUIRE ( map.get(111) );
  REQUIRE ( map.get(176 ));
  REQUIRE ( map.get(255) );
}
