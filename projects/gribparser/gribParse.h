#ifndef GRIB_PARSER_FILE_PARSE_H
#define GRIB_PARSER_FILE_PARSE_H

#include <eccodes.h>
#include <string>
#include <iostream>
#include <iostream>
#include "../src/logic/StandardCalc.h"
using namespace std;

class FileParse {
    public:
        double *lats_, *lons_, *values_;
        long number_of_points_;
        const double kMissing = 9999.0;

        FileParse(const std::string filename);

    private:
        int err;
        FILE *in;
        codes_handle *lib_handle;

};

#endif //GRIB_PARSER_FILE_PARSE_H
