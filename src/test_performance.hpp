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

enum weather_event { fog, rain, thunderstorm, snow };

class weather_point
{
public:
    time_t date;
    int temp_max, temp_mean, temp_min;
    int dew_max, dew_mean, dew_min;
    int humidity_max, humidity_mean, humidity_min;
    double pressure_max, pressure_mean, pressure_min;
    int visibility_max, visibility_mean, visibility_min;
    int windspeed_max, windspeed_mean;
    int gustspeed_max;
    double precipitation;
    int cloud_cover;
    vector<weather_event> events;
    int wind_direction;
};

vector<weather_point> load_weather(string path);

#endif
