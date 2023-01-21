#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include "../../src/glow/Grid.h"

using namespace glow;

TEST_CASE("Grid Map Diagonal 2x2", "[map_2x2]")
{
  Properties properties;
  properties(4, 2.0, TopLeft, Diagonal);
  Grid grid;
  grid.setup(properties);

  REQUIRE(1 == grid.First());
  REQUIRE(1 == grid.Offset());

  REQUIRE(0 == grid.map(0));
  REQUIRE(1 == grid.map(1));
  REQUIRE(2 == grid.map(2));
  REQUIRE(3 == grid.map(3));
}

TEST_CASE("Grid Map Diagonal 2x3", "[map_2x3]")
{
  Properties properties;
  properties(6.0, 2.0, TopLeft, Diagonal);
  Grid grid;
  grid.setup(properties);

  REQUIRE(1 == grid.First());
  REQUIRE(1 == grid.Offset());
  REQUIRE(4 == grid.Last());

  REQUIRE(0 == grid.map(0));
  REQUIRE(1 == grid.map(1));
  REQUIRE(3 == grid.map(2));
  REQUIRE(2 == grid.map(3));
  REQUIRE(4 == grid.map(4));
  REQUIRE(5 == grid.map(5));
}

TEST_CASE("Grid Map Diagonal 3x2", "[map_3x2]")
{
  Properties properties;
  properties(6, 3.0, TopLeft, Diagonal);
  Grid grid;
  grid.setup(properties);

  REQUIRE(1 == grid.First());
  REQUIRE(1 == grid.Offset());

  REQUIRE(0 == grid.map(0));
  REQUIRE(1 == grid.map(1));
  REQUIRE(2 == grid.map(2));
  REQUIRE(3 == grid.map(3));
  REQUIRE(4 == grid.map(4));
  REQUIRE(5 == grid.map(5));
}

TEST_CASE("Grid Map Diagonal 3x4", "[map_3x4]")
{
  Properties properties;
  properties(12, 3.0, TopLeft, Diagonal);
  Grid grid;
  grid.setup(properties);

  REQUIRE(3 == grid.First());
  REQUIRE(2 == grid.Offset());
  REQUIRE(8 == grid.Last());

  REQUIRE(0 == grid.map(0));
  REQUIRE(1 == grid.map(1));
  REQUIRE(4 == grid.map(2));
  REQUIRE(2 == grid.map(3));
  REQUIRE(5 == grid.map(4));
  REQUIRE(8 == grid.map(5));
  REQUIRE(3 == grid.map(6));
  REQUIRE(6 == grid.map(7));
  REQUIRE(9 == grid.map(8));
  REQUIRE(7 == grid.map(9));
  REQUIRE(10 == grid.map(10));
  REQUIRE(11 == grid.map(11));
}

TEST_CASE("Grid Map Diagonal 4x5", "[map_4x5]")
{
  Properties properties;
  properties(20, 4.0, TopLeft, Diagonal);
  Grid grid;
  grid.setup(properties);

  REQUIRE(6 == grid.First());
  REQUIRE(3 == grid.Offset());
  REQUIRE(13 == grid.Last());

  REQUIRE(0 == grid.map(0));
  REQUIRE(1 == grid.map(1));
  REQUIRE(5 == grid.map(2));
  REQUIRE(2 == grid.map(3));
  REQUIRE(6 == grid.map(4));
  REQUIRE(10 == grid.map(5));
  REQUIRE(3 == grid.map(6));
  REQUIRE(7 == grid.map(7));
  REQUIRE(11 == grid.map(8));
  REQUIRE(15 == grid.map(9));
  REQUIRE(4 == grid.map(10));
  REQUIRE(8 == grid.map(11));
  REQUIRE(12 == grid.map(12));
  REQUIRE(16 == grid.map(13));
  REQUIRE(9 == grid.map(14));
  REQUIRE(13 == grid.map(15));
  REQUIRE(17 == grid.map(16));
  REQUIRE(14 == grid.map(17));
  REQUIRE(18 == grid.map(18));
  REQUIRE(19 == grid.map(19));
}

TEST_CASE("Grid Map Diagonal 4x9", "[map_49]")
{
  Properties properties;
  properties(36, 4.0, TopLeft, Diagonal);
  Grid grid;
  grid.setup(properties);

  REQUIRE(6 == grid.First());
  REQUIRE(3 == grid.Offset());
  REQUIRE(29 == grid.Last());

  // repeat to check consistency
  grid.setup(properties);
  REQUIRE(6 == grid.First());
  REQUIRE(3 == grid.Offset());
  REQUIRE(29 == grid.Last());

  REQUIRE(0 == grid.map(0));
  REQUIRE(1 == grid.map(1));
  REQUIRE(9 == grid.map(2));
  REQUIRE(2 == grid.map(3));
  REQUIRE(10 == grid.map(4));
  REQUIRE(18 == grid.map(5));
  REQUIRE(3 == grid.map(6));
  REQUIRE(11 == grid.map(7));
  REQUIRE(19 == grid.map(8));
  REQUIRE(27 == grid.map(9));
  REQUIRE(4 == grid.map(10));
  REQUIRE(12 == grid.map(11));
  REQUIRE(20 == grid.map(12));
  REQUIRE(28 == grid.map(13));
  REQUIRE(5 == grid.map(14));
  REQUIRE(13 == grid.map(15));
  REQUIRE(21 == grid.map(16));
  REQUIRE(29 == grid.map(17));
  REQUIRE(6 == grid.map(18));
  REQUIRE(14 == grid.map(19));
  REQUIRE(22 == grid.map(20));
  REQUIRE(30 == grid.map(21));
  REQUIRE(7 == grid.map(22));
  REQUIRE(15 == grid.map(23));
  REQUIRE(23 == grid.map(24));
  REQUIRE(31 == grid.map(25));
  REQUIRE(8 == grid.map(26));
  REQUIRE(16 == grid.map(27));
  REQUIRE(24 == grid.map(28));
  REQUIRE(32 == grid.map(29));

  REQUIRE(17 == grid.map(30));
  REQUIRE(25 == grid.map(31));

  REQUIRE(33 == grid.map(32));
  REQUIRE(26 == grid.map(33));
  REQUIRE(34 == grid.map(34));
  REQUIRE(35 == grid.map(35));
}

TEST_CASE("Grid Map Diagonal-TopRight 4x9", "[map_mirror_49]")
{
  Properties properties;
  properties(36, 4.0, TopRight, Diagonal);
  Grid grid;
  grid.setup(properties);

  REQUIRE(6 == grid.First());
  REQUIRE(3 == grid.Offset());
  REQUIRE(29 == grid.Last());

  REQUIRE(8 == grid.map(0));
  REQUIRE(7 == grid.map(1));
  REQUIRE(17 == grid.map(2));
  REQUIRE(6 == grid.map(3));
  REQUIRE(16 == grid.map(4));

  REQUIRE(26 == grid.map(5));
  REQUIRE(5 == grid.map(6));
  REQUIRE(15 == grid.map(7));
  REQUIRE(25 == grid.map(8));
  REQUIRE(35 == grid.map(9));

  REQUIRE(4 == grid.map(10));
  REQUIRE(14 == grid.map(11));
  REQUIRE(24 == grid.map(12));
  REQUIRE(34 == grid.map(13));
  REQUIRE(18 == grid.map(33));
  REQUIRE(28 == grid.map(34));
  REQUIRE(27 == grid.map(35));
}

TEST_CASE("Grid Map Diagonal-BottomRight 4x9", "[map_49]")
{
  Properties properties;
  properties(36, 4.0, BottomRight, Diagonal);
  Grid grid;
  grid.setup(properties);

  REQUIRE(6 == grid.First());
  REQUIRE(3 == grid.Offset());
  REQUIRE(29 == grid.Last());

  REQUIRE(35 == grid.map(0));
  REQUIRE(34 == grid.map(1));
  REQUIRE(26 == grid.map(2));
  REQUIRE(33 == grid.map(3));
  REQUIRE(25 == grid.map(4));
  REQUIRE(17 == grid.map(5));
  REQUIRE(32 == grid.map(6));
  REQUIRE(24 == grid.map(7));
  REQUIRE(16 == grid.map(8));
  REQUIRE(8 == grid.map(9));
  REQUIRE(31 == grid.map(10));
  REQUIRE(23 == grid.map(11));
  REQUIRE(15 == grid.map(12));
  REQUIRE(7 == grid.map(13));
  REQUIRE(30 == grid.map(14));
  REQUIRE(22 == grid.map(15));
  REQUIRE(14 == grid.map(16));
  REQUIRE(6 == grid.map(17));
  REQUIRE(29 == grid.map(18));
  REQUIRE(21 == grid.map(19));
  REQUIRE(13 == grid.map(20));
  REQUIRE(5 == grid.map(21));
  REQUIRE(28 == grid.map(22));
  REQUIRE(20 == grid.map(23));
  REQUIRE(12 == grid.map(24));
  REQUIRE(4 == grid.map(25));
  REQUIRE(27 == grid.map(26));
  REQUIRE(19 == grid.map(27));
  REQUIRE(11 == grid.map(28));
  REQUIRE(3 == grid.map(29));

  REQUIRE(18 == grid.map(30));
  REQUIRE(10 == grid.map(31));

  REQUIRE(2 == grid.map(32));
  REQUIRE(9 == grid.map(33));
  REQUIRE(1 == grid.map(34));
  REQUIRE(0 == grid.map(35));
}

TEST_CASE("Grid Map Diagonal-BottomLeft 4x9", "[map_49]")
{
  Properties properties;
  properties(36, 4.0, BottomLeft, Diagonal);
  Grid grid;
  grid.setup(properties);

  REQUIRE(6 == grid.First());
  REQUIRE(3 == grid.Offset());
  REQUIRE(29 == grid.Last());

  REQUIRE(27 == grid.map(0));
  REQUIRE(28 == grid.map(1));
  REQUIRE(18 == grid.map(2));
  REQUIRE(29 == grid.map(3));
  REQUIRE(19 == grid.map(4));
  REQUIRE(9 == grid.map(5));
  REQUIRE(30 == grid.map(6));
  REQUIRE(20 == grid.map(7));
  REQUIRE(10 == grid.map(8));
  REQUIRE(0 == grid.map(9));
  REQUIRE(31 == grid.map(10));
  REQUIRE(21 == grid.map(11));
  REQUIRE(11 == grid.map(12));
  REQUIRE(1 == grid.map(13));
  REQUIRE(32 == grid.map(14));
  REQUIRE(22 == grid.map(15));
  REQUIRE(12 == grid.map(16));
  REQUIRE(2 == grid.map(17));
  REQUIRE(33 == grid.map(18));
  REQUIRE(23 == grid.map(19));
  REQUIRE(13 == grid.map(20));
  REQUIRE(3 == grid.map(21));
  REQUIRE(34 == grid.map(22));
  REQUIRE(24 == grid.map(23));
  REQUIRE(14 == grid.map(24));
  REQUIRE(4 == grid.map(25));
  REQUIRE(35 == grid.map(26));
  REQUIRE(25 == grid.map(27));
  REQUIRE(15 == grid.map(28));
  REQUIRE(5 == grid.map(29));

  REQUIRE(26 == grid.map(30));
  REQUIRE(16 == grid.map(31));
  REQUIRE(6 == grid.map(32));
  REQUIRE(17 == grid.map(33));
  REQUIRE(7 == grid.map(34));
  REQUIRE(8 == grid.map(35));
}
