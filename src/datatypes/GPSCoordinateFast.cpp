// Copyright 2016 UBC Sailbot

#include "datatypes/GPSCoordinateFast.h"

#include <logic/StandardCalc.h>

GPSCoordinateFast::GPSCoordinateFast() {}
GPSCoordinateFast::GPSCoordinateFast(int32_t latitude_exact, int32_t longitude_exact) {
  set_lat_lng_exact(latitude_exact, longitude_exact);
}
GPSCoordinateFast::GPSCoordinateFast(const GPSCoordinate &coordinate) {
  set_lat_lng_exact(coordinate.latitude_exact(), coordinate.longitude_exact());
}

void GPSCoordinateFast::set_lat_exact(int32_t latitude_exact) {
  latitude_exact_ = latitude_exact;
  latitude_ = standard_calc::deg_to_rad(latitude_exact_ / static_cast<float>(kExactCoordinateScaleFactor));
}

void GPSCoordinateFast::set_lng_exact(int32_t longitude_exact) {
  longitude_exact_ = longitude_exact;
  longitude_ = standard_calc::deg_to_rad(longitude_exact_ / static_cast<float>(kExactCoordinateScaleFactor));
}

void GPSCoordinateFast::set_lat_lng_exact(int32_t latitude_exact, int32_t longitude_exact) {
  set_lat_exact(latitude_exact);
  set_lng_exact(longitude_exact);
}

void GPSCoordinateFast::set_waypoint(const GPSCoordinate &coordinate) {
  set_lat_lng_exact(coordinate.latitude_exact(), coordinate.longitude_exact());
  set_waypoint_level(coordinate.waypoint_level());
}
