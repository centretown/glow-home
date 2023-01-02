#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include "../../src/glow/grid.h"

using namespace glow;

TEST_CASE("Grid Map Diagonal 2x2", "[map_diagonal_2x2]")
{
  Grid grid;
  grid.setup(4, 2);

  REQUIRE(1 == grid.pivot_first);
  REQUIRE(1 == grid.pivot_offset);

  REQUIRE(0 == grid.map_diagonal(0));
  REQUIRE(1 == grid.map_diagonal(1));
  REQUIRE(2 == grid.map_diagonal(2));
  REQUIRE(3 == grid.map_diagonal(3));
}

TEST_CASE("Grid Map Diagonal 2x3", "[map_diagonal_2x3]")
{
  Grid grid;
  grid.setup(6, 2);
  REQUIRE(1 == grid.pivot_first);
  REQUIRE(1 == grid.pivot_offset);
  REQUIRE(4 == grid.pivot_last);

  REQUIRE(0 == grid.map_diagonal(0));
  REQUIRE(1 == grid.map_diagonal(1));
  REQUIRE(3 == grid.map_diagonal(2));
  REQUIRE(2 == grid.map_diagonal(3));
  REQUIRE(4 == grid.map_diagonal(4));
  REQUIRE(5 == grid.map_diagonal(5));
}

TEST_CASE("Grid Map Diagonal 3x2", "[map_diagonal_3x2]")
{
  Grid grid;
  grid.setup(6, 3);
  REQUIRE(1 == grid.pivot_first);
  REQUIRE(1 == grid.pivot_offset);

  REQUIRE(0 == grid.map_diagonal(0));
  REQUIRE(1 == grid.map_diagonal(1));
  REQUIRE(2 == grid.map_diagonal(2));
  REQUIRE(3 == grid.map_diagonal(3));
  REQUIRE(4 == grid.map_diagonal(4));
  REQUIRE(5 == grid.map_diagonal(5));
}

TEST_CASE("Grid Map Diagonal 3x4", "[map_diagonal_3x4]")
{
  Grid grid;
  grid.setup(12, 3);
  REQUIRE(3 == grid.pivot_first);
  REQUIRE(2 == grid.pivot_offset);
  REQUIRE(8 == grid.pivot_last);

  REQUIRE(0 == grid.map_diagonal(0));
  REQUIRE(1 == grid.map_diagonal(1));
  REQUIRE(4 == grid.map_diagonal(2));
  REQUIRE(2 == grid.map_diagonal(3));
  REQUIRE(5 == grid.map_diagonal(4));
  REQUIRE(8 == grid.map_diagonal(5));
  REQUIRE(3 == grid.map_diagonal(6));
  REQUIRE(6 == grid.map_diagonal(7));
  REQUIRE(9 == grid.map_diagonal(8));
  REQUIRE(7 == grid.map_diagonal(9));
  REQUIRE(10 == grid.map_diagonal(10));
  REQUIRE(11 == grid.map_diagonal(11));
}

TEST_CASE("Grid Map Diagonal 4x5", "[map_diagonal_4x5]")
{
  Grid grid;
  grid.setup(20, 4);
  REQUIRE(6 == grid.pivot_first);
  REQUIRE(3 == grid.pivot_offset);
  REQUIRE(13 == grid.pivot_last);

  REQUIRE(0 == grid.map_diagonal(0));
  REQUIRE(1 == grid.map_diagonal(1));
  REQUIRE(5 == grid.map_diagonal(2));
  REQUIRE(2 == grid.map_diagonal(3));
  REQUIRE(6 == grid.map_diagonal(4));
  REQUIRE(10 == grid.map_diagonal(5));
  REQUIRE(3 == grid.map_diagonal(6));
  REQUIRE(7 == grid.map_diagonal(7));
  REQUIRE(11 == grid.map_diagonal(8));
  REQUIRE(15 == grid.map_diagonal(9));
  REQUIRE(4 == grid.map_diagonal(10));
  REQUIRE(8 == grid.map_diagonal(11));
  REQUIRE(12 == grid.map_diagonal(12));
  REQUIRE(16 == grid.map_diagonal(13));
  REQUIRE(9 == grid.map_diagonal(14));
  REQUIRE(13 == grid.map_diagonal(15));
  REQUIRE(17 == grid.map_diagonal(16));
  REQUIRE(14 == grid.map_diagonal(17));
  REQUIRE(18 == grid.map_diagonal(18));
  REQUIRE(19 == grid.map_diagonal(19));
}

TEST_CASE("Grid Map Diagonal 4x9", "[map_diagonal_49]")
{
  Grid grid;
  grid.setup(36, 4);
  REQUIRE(6 == grid.pivot_first);
  REQUIRE(3 == grid.pivot_offset);
  REQUIRE(29 == grid.pivot_last);

  REQUIRE(0 == grid.map_diagonal(0));
  REQUIRE(1 == grid.map_diagonal(1));
  REQUIRE(9 == grid.map_diagonal(2));
  REQUIRE(2 == grid.map_diagonal(3));
  REQUIRE(10 == grid.map_diagonal(4));
  REQUIRE(18 == grid.map_diagonal(5));
  REQUIRE(3 == grid.map_diagonal(6));
  REQUIRE(11 == grid.map_diagonal(7));
  REQUIRE(19 == grid.map_diagonal(8));
  REQUIRE(27 == grid.map_diagonal(9));
  REQUIRE(4 == grid.map_diagonal(10));
  REQUIRE(12 == grid.map_diagonal(11));
  REQUIRE(20 == grid.map_diagonal(12));
  REQUIRE(28 == grid.map_diagonal(13));
  REQUIRE(5 == grid.map_diagonal(14));
  REQUIRE(13 == grid.map_diagonal(15));
  REQUIRE(21 == grid.map_diagonal(16));
  REQUIRE(29 == grid.map_diagonal(17));
  REQUIRE(6 == grid.map_diagonal(18));
  REQUIRE(14 == grid.map_diagonal(19));
  REQUIRE(22 == grid.map_diagonal(20));
  REQUIRE(30 == grid.map_diagonal(21));
  REQUIRE(7 == grid.map_diagonal(22));
  REQUIRE(15 == grid.map_diagonal(23));
  REQUIRE(23 == grid.map_diagonal(24));
  REQUIRE(31 == grid.map_diagonal(25));
  REQUIRE(8 == grid.map_diagonal(26));
  REQUIRE(16 == grid.map_diagonal(27));
  REQUIRE(24 == grid.map_diagonal(28));
  REQUIRE(32 == grid.map_diagonal(29));

  REQUIRE(17 == grid.map_diagonal(30));
  REQUIRE(25 == grid.map_diagonal(31));

  REQUIRE(33 == grid.map_diagonal(32));
  REQUIRE(26 == grid.map_diagonal(33));
  REQUIRE(34 == grid.map_diagonal(34));
  REQUIRE(35 == grid.map_diagonal(35));
}
