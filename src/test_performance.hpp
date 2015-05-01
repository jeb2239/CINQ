#ifndef __test_performance_hpp__
#define __test_performance_hpp__

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>

#include "cinq_enumerable.hpp"
#include "test_shared.hpp"

using namespace std;

vector<test_perf> make_tests_perf();

enum weather_event { unrecognized, fog, rain, thunderstorm, snow };

class weather_point
{
public:
    tm date;
    int temp_max, temp_avg, temp_min;
    int dew_max, dew_avg, dew_min;
    int humidity_max, humidity_avg, humidity_min;
    double pressure_max, pressure_avg, pressure_min;
    int visibility_max, visibility_avg, visibility_min;
    int windspeed_max, windspeed_avg;
    int gustspeed_max;
    double precipitation;
    int cloud_cover;
    bool rain,thunderstorm,snow,fog;
    int wind_direction;
};

vector<weather_point> load_weather(string path);

#endif
