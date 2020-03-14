// Copyright 2017 UBC Sailbot
#include "gribParse.h"
#include <vector>
#include <grib/UrlBuilder.h>
#include <grib/UrlDownloader.h>
#include <iomanip>
#include <fstream>

/**
 * Translates GRIB file into array of lattitudes, longitudes, and corresponding values
 * @param filename of target GRIB file
 * @return
 */

gribParse::gribParse(const std::string filename) {
  err = 0;
  in = fopen(filename.c_str(), "r");
  if (!in) {
    std::cout << "ERROR: unable to open input file" << filename << std::endl;
  }

  std::vector<std::vector<double>> u_values;
  std::vector<std::vector<double>> v_values;

  int time_steps_ = 5;

  for (int code_handle_iteration = 1; ((lib_handle = codes_handle_new_from_file(0, in, PRODUCT_GRIB, &err)) != NULL) && code_handle_iteration <= 31; code_handle_iteration++) {
      CODES_CHECK(err, 0);
      CODES_CHECK (codes_get_long(lib_handle, "numberOfPoints", &number_of_points_), 0);
      CODES_CHECK(codes_set_double(lib_handle, "missingValue", kMissing), 0);
      lats.resize(number_of_points_);
      lons.resize(number_of_points_);


      switch(code_handle_iteration) {
          case 1:
              cape.resize(number_of_points_);
              CODES_CHECK(codes_grib_get_data(lib_handle,lats.data(), lons.data(), cape.data()), 0);
              break;
          case 2:
              wind_gust.resize(number_of_points_);
              CODES_CHECK(codes_grib_get_data(lib_handle,lats.data(), lons.data(), wind_gust.data()), 0);
              break;
          case 3:
              precipitation.resize(number_of_points_);
              CODES_CHECK(codes_grib_get_data(lib_handle,lats.data(), lons.data(), precipitation.data()), 0);
              break;
          case 4:
              pressure.resize(number_of_points_);
              CODES_CHECK(codes_grib_get_data(lib_handle,lats.data(), lons.data(), pressure.data()), 0);
              break;
          case 5:
              cloudcover.resize(number_of_points_);
              CODES_CHECK(codes_grib_get_data(lib_handle,lats.data(), lons.data(), cloudcover.data()), 0);
              break;
          case 6:
              temperature.resize(number_of_points_);
              CODES_CHECK(codes_grib_get_data(lib_handle,lats.data(), lons.data(), temperature.data()), 0);
              break;
          case 7:
              u_values.resize(time_steps_);
              u_values[0].resize(number_of_points_);
              CODES_CHECK(codes_grib_get_data(lib_handle,lats.data(), lons.data(), u_values[0].data()), 0);
              break;
          case 8:
              v_values.resize(time_steps_);
              v_values[0].resize(number_of_points_);
              CODES_CHECK(codes_grib_get_data(lib_handle,lats.data(), lons.data(), v_values[0].data()), 0);
              std::cout << "0";
              break;
          case 15:
              u_values.resize(time_steps_);
              u_values[1].resize(number_of_points_);
              CODES_CHECK(codes_grib_get_data(lib_handle,lats.data(), lons.data(), u_values[1].data()), 0);
              std::cout << "1";
              break;
          case 16:
              v_values.resize(time_steps_);
              v_values[1].resize(number_of_points_);
              CODES_CHECK(codes_grib_get_data(lib_handle,lats.data(), lons.data(), v_values[1].data()), 0);
              break;
          case 23:
              u_values.resize(time_steps_);
              u_values[2].resize(number_of_points_);
              CODES_CHECK(codes_grib_get_data(lib_handle,lats.data(), lons.data(), u_values[2].data()), 0);
              break;
          case 24:
              v_values.resize(time_steps_);
              v_values[2].resize(number_of_points_);
              CODES_CHECK(codes_grib_get_data(lib_handle,lats.data(), lons.data(), v_values[2].data()), 0);
              break;
          case 31:
              u_values.resize(time_steps_);
              u_values[3].resize(number_of_points_);
              CODES_CHECK(codes_grib_get_data(lib_handle,lats.data(), lons.data(), u_values[3].data()), 0);
              break;
          case 32:
              v_values.resize(time_steps_);
              v_values[3].resize(number_of_points_);
              CODES_CHECK(codes_grib_get_data(lib_handle,lats.data(), lons.data(), v_values[3].data()), 0);
              break;
          default:
              break;

      }
      codes_handle_delete(lib_handle);
  }
  angles.resize(time_steps_);
  magnitudes.resize(time_steps_);
  missing.resize(time_steps_);

  u_values.resize(time_steps_);
  v_values.resize(time_steps_);


    CODES_CHECK(codes_set_double(lib_handle, "missingValue", kMissing), 0);

    //adjust latitude and longitude, and generate resultant angles and magnitudes
   for(int i = 0; i < time_steps_; i++) {
      angles[i].resize(number_of_points_);
      magnitudes[i].resize(number_of_points_);
      missing[i].resize(number_of_points_);
      u_values[i].resize(number_of_points_);
      v_values[i].resize(number_of_points_);
      for (int j = 0; j < number_of_points_; j++) {
          if(u_values[i][j] == kMissing)
              missing[i][j] = true;
          else
              missing[i][j] = false;

          angles[i][j] = calcAngle(u_values[i][j],v_values[i][j]);
          magnitudes[i][j] = calcMagnitude(u_values[i][j],v_values[i][j]);
          lats[j] = standard_calc::BoundTo180(lats[j]);
          lons[j] = standard_calc::BoundTo180(lons[j]);
      }
    }
  fclose(in);
}

/**
 * Calculates angle, relative to an origin at the 12 o'clock position
 * @param u and v vector component of the wind
 * @return angle in degrees, relative to new position
 */
double gribParse::calcAngle(double u_comp, double v_comp) {
    return 270-(atan(v_comp/u_comp) * 180 / PI);
}

/**
 * Calculates magnitude of wind vector
 * @param u and v vector component of the wind
 * @return magnitude of vector in knots/s
 */
double gribParse::calcMagnitude(double u_comp, double v_comp) {
    return (sqrt(pow(u_comp,2) + pow(v_comp,2)))/0.514444;
}

void gribParse::saveKML() {
    std::ofstream ss;
    ss.open("Wind.kml");
    ss << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
          "<kml xmlns=\"http://earth.google.com/kml/2.0\">\n"
          "<Document><name>Wind</name><Folder>\n";
    std::string color;
    int time_step;

    std::string darkGreenArrow = "<href>https://upload.wikimedia.org/wikipedia/commons/thumb/3/30/Green_Arrow_Down_Darker.svg/1000px-Green_Arrow_Down_Darker.svg.png</href>";
    std::string redArrow = "<href>https://upload.wikimedia.org/wikipedia/commons/thumb/0/04/Red_Arrow_Down.svg/1024px-Red_Arrow_Down.svg.png</href>";
    std::string greenArrow = "<href>https://upload.wikimedia.org/wikipedia/commons/thumb/6/6e/Green_Arrow_Down.svg/1000px-Green_Arrow_Down.svg.png</href>";
    std::string whiteArrow = "<href>https://upload.wikimedia.org/wikipedia/commons/thumb/f/ff/White_Arrow_Down.svg/560px-White_Arrow_Down.svg.png</href>";

    for (int i = 0; i < angles[0].size(); i++) {
      double dist = sqrt(pow(lats[i]-lats[angles[0].size()-1],2)+pow(lons[i]-lons[angles[0].size()-1],2));

      if(dist < 2){
        color = darkGreenArrow;
        time_step = 0;
      }
      else if(dist < 4){
        color = redArrow;
        time_step = 1;
      }
      else if(dist < 6){
        color = greenArrow;
        time_step = 2;
      }
      else{
        color = whiteArrow;
        time_step = 3;
      }

        ss << "<GroundOverlay>"
            "<color>ffffffff</color>"
		      "<drawOrder>1</drawOrder>"
          "<Icon>"
          <<color<<
          "<refreshMode>onInterval</refreshMode>"
		      "<refreshInterval>86400</refreshInterval>"
		      "<viewBoundScale>0.75</viewBoundScale>"
		      "</Icon>"
		      "<LatLonBox>"
		      "<north>" << lats[i] + magnitudes[time_step][i]/80 << "</north>"
		      "<south>" << lats[i] - magnitudes[time_step][i]/80 << "</south>"
		      "<east>" << lons[i] + magnitudes[time_step][i]/80 << "</east>"
		      "<west>" << lons[i] - magnitudes[time_step][i]/80 << "</west>"
		      "<rotation>" << 360-angles[time_step][i] << "</rotation>"
		      "</LatLonBox>"
		      "</GroundOverlay>" << std::endl;
    }
    ss << "</Folder>\n</Document>\n</kml>" << std::endl;

    ss.close();
}
