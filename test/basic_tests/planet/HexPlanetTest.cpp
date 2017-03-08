// Copyright 2017 UBC Sailbot

#include "HexPlanetTest.h"

#include <planet/HexPlanet.h>

/// The planet subdivision count used for tests
static constexpr int kTestPlanetSize = 6;
static constexpr size_t kTestPlanetVertexCount = 7292;
static constexpr size_t kTestPlanetTriangleCount = 14580;

HexPlanetTest::HexPlanetTest() {}

TEST_F(HexPlanetTest, HexPlanetCreationTest) {
  HexPlanet hex_planet = HexPlanet(kTestPlanetSize);
  EXPECT_EQ(hex_planet.vertex_count(), kTestPlanetVertexCount);
  EXPECT_EQ(hex_planet.triangle_count(), kTestPlanetTriangleCount);
}

/**
 * This is a test of the UpdateVertexNeighbours method, which is run in the HexPlanet constructor.
 * Check that for every vertex "i" in the HexPlanet, every neighbour of vertex "i" contains "i" as a neighbour.
 * This is done with multiple subdivisions.
 */
TEST_F(HexPlanetTest, ComputeVertexNeighboursTest) {
  // Run the test with sub divisions ranging from 0 to kTestPlanetSize
  for (int subdivisions = 0; subdivisions <= kTestPlanetSize; subdivisions++) {
    HexPlanet hex_planet = HexPlanet(subdivisions);

    // Check every vertex in the hexplanet
    for (size_t i = 0; i < hex_planet.vertex_count(); i++) {
      HexVertex current_vertex = hex_planet.vertex(i);

      auto neighbours = current_vertex.neighbours;

      // The number of neighbours of a vertex must be 5 or 6
      EXPECT_TRUE(current_vertex.neighbour_count == 5 || current_vertex.neighbour_count == 6);

      // If there are 5 valid neighbours, check that the last vertex is kInvalidHexVertexId
      if (current_vertex.neighbour_count == 5) {
        EXPECT_EQ(kInvalidHexVertexId, neighbours.at(5));
      }

      // Check every neighbour of vertex i, and make sure it contains i as a neighbour
      for (HexVertexId j = 0; j < current_vertex.neighbour_count; j++) {
        HexVertexId neighbour_id = neighbours.at(j);

        auto neighbours_of_neighbour = hex_planet.vertex(neighbour_id).neighbours;

        // We expect vertex i to be a neighbour, since this vertex (vertex "id") is one of neighbours of i.
        EXPECT_TRUE(std::find(neighbours_of_neighbour.begin(), neighbours_of_neighbour.end(), i) !=
            neighbours_of_neighbour.end());
      }
    }
  }
}
