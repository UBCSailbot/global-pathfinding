// Copyright 2017 UBC Sailbot

#ifndef PATHFINDING_PATHFINDERRESULTPRINTER_H_
#define PATHFINDING_PATHFINDERRESULTPRINTER_H_

#include <vector>

#include "pathfinding/Pathfinder.h"
#include "datatypes/HexDefs.h"

class PathfinderResultPrinter {
 public:
  /**
   * Produces a verbose output string containing the cost, ending time step and then a space-separated list of vertex
   * ids.
   * @param planet Planet corresponding to the result.
   * @param result Pathfinding result to be printed.
   * @return Generated output string.
   */
  static std::string PrintDefault(const Pathfinder::Result &result);

  /**
   * Produces a string containing the vertex index, lat, lng of the result path, each on its own line.
   * @param planet Planet corresponding to the result.
   * @param result Pathfinding result to be printed.
   * @return Generated output string.
   */
  static std::string PrintCoordinates(HexPlanet &planet, const Pathfinder::Result &result);

  /**
   * Produces a KML formatted line.
   * @param planet Planet corresponding to the result.
   * @param result Pathfinding result to be printed.
   * @return Generated KML output string.
   */
  static std::string PrintKML(HexPlanet &planet, const Pathfinder::Result &result);
};

#endif  // PATHFINDING_PATHFINDERRESULTPRINTER_H_
