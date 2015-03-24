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
