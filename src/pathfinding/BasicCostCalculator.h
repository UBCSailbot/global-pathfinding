// Copyright 2017 UBC Sailbot

#ifndef PATHFINDING_BASICCOSTCALCULATOR_H_
#define PATHFINDING_BASICCOSTCALCULATOR_H_

#include "pathfinding/BasicHexMap.h"
#include "pathfinding/CostCalculator.h"
#include "planet/HexPlanet.h"

class BasicCostCalculator : public CostCalculator {
 public:
  /**
   * Creates a BasicCostCalculator instance that gets the cost from one point
   * to another. The Calculator will take ownership of the map it is given and
   * handle its deletion.
   * @param planet The planet.
   * @param map The risk map for the planet.
   */
  explicit BasicCostCalculator(HexPlanet &planet, std::unique_ptr<BasicHexMap> &map);

  /**
   * Computes the a distance between two points using the Haversine formula and the BasicHexMap.
   * Note: Currently just increments time by one.
   * @param source Source vertex ID.
   * @param target Target vertex ID.
   * @param time Starting time step.
   * @throw std::runtime_error if target or source does not exist on the planet.
   * @return The cost (distance in meters + BasicHexMap based cost) and ending time step for an edge.
   */
  Result calculate(HexVertexId target, HexVertexId source, uint32_t start_time) const override;

  // Class can't be copied
  BasicCostCalculator(const BasicCostCalculator &) = delete;

  // Class can't be moved
  BasicCostCalculator(BasicCostCalculator &&) = delete;

  // Class can't be copy assigned
  BasicCostCalculator &operator=(const BasicCostCalculator &) = delete;

  // Class can't be move assigned
  BasicCostCalculator &operator=(BasicCostCalculator &&) = delete;

 private:
  std::unique_ptr<BasicHexMap> map_;
};

#endif  // PATHFINDING_BASICCOSTCALCULATOR_H_
