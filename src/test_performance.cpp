#include "test_performance.hpp"

vector<test_perf> make_tests_perf()
{
    vector<weather_point> weather_data = load_weather("../data/weather_kjfk_1948-2014.csv");
    
    vector<test_perf> tests;
    
    tests.push_back(test_perf("where() by temperature", [=]
    {
        cinq::from(weather_data)
              .where([](auto& x) -> bool { return x.temp_max > 90; });
    }));
    
    tests.push_back(test_perf("where() by temperature - manual", [=]
    {
        vector<weather_point> result;
        for (auto data : weather_data)
        {
            if (data.temp_max > 90) result.push_back(data);
        }
    }));

    tests.push_back(test_perf("select() mapping weather_point to cloud_cover", [=]
    {
       cinq::from(weather_data).select([](auto& x){return x.cloud_cover;});
    }));
    
    return tests;
}

vector<string> split(const string &str, char delimiter)
{
    stringstream ss(str);
    string item;
    vector<string> elems;
    while (getline(ss, item, delimiter))
    {
        elems.push_back(item);
    }
    return elems;
}

string fix(string s)
{
    if (s == "" || s == "T") return "0";
    else return s;
}

vector<weather_point> load_weather(string path)
{
    ifstream source(path, ios::in);
    vector<weather_point> parsed;
    
    string tmp;
    getline(source, tmp);
    vector<string> headers = split(tmp, ',');
    
    while (getline(source, tmp))
    {
        // Get a line and put it in a dictionary
        vector<string> cells = split(tmp, ',');
        unordered_map<string, string> line;
        if (cells.size() != headers.size()) break;
        for (size_t i = 0; i < cells.size(); i++)
        {
            line[headers[i]] = cells[i];
            //printf("%s = %s\n", headers[i].c_str(), cells[i].c_str());
        }
        //printf("\n");
        
        weather_point p;
        
        if (!strptime(line["EST"].c_str(), "%Y-%m-%d", &p.date)) printf("parse error\n");
        
        p.temp_max = stoi(fix(line["Max TemperatureF"]));
        p.temp_avg = stoi(fix(line["Mean TemperatureF"]));
        p.temp_min = stoi(fix(line["Min TemperatureF"]));
        
        // These next 3 lines look wrong, but that's actually how Wunderground names their column headers.
        p.dew_max = stoi(fix(line["Max Dew PointF"]));
        p.dew_avg = stoi(fix(line["MeanDew PointF"]));
        p.dew_min = stoi(fix(line["Min DewpointF"]));
        
        p.humidity_max = stoi(fix(line["Max Humidity"]));
        p.humidity_avg = stoi(fix(line["Mean Humidity"]));
        p.humidity_min = stoi(fix(line["Min Humidity"]));
        
        p.pressure_max = stod(fix(line["Max Sea Level PressureIn"]));
        p.pressure_avg = stod(fix(line["Mean Sea Level PressureIn"]));
        p.pressure_min = stod(fix(line["Min Sea Level PressureIn"]));
        
        p.visibility_max = stoi(fix(line["Max VisibilityMiles"]));
        p.visibility_avg = stoi(fix(line["Mean VisibilityMiles"]));
        p.visibility_min = stoi(fix(line["Min VisibilityMiles"]));
        
        p.windspeed_max = stoi(fix(line["Max Wind SpeedMPH"]));
        p.windspeed_avg = stoi(fix(line["Mean Wind SpeedMPH"]));
        
        p.gustspeed_max = stoi(fix(line["Max Gust SpeedMPH"]));
        
        p.precipitation = stod(fix(line["PrecipitationIn"]));
        
        p.cloud_cover = stoi(fix(line["CloudCover"]));
        
        for (string event : split(line["Events"], '-'))
        {
            weather_event e = unrecognized;
            
            if (event == "Fog") e = fog;
            else if (event == "Rain") e = rain;
            else if (event == "Thunderstorm") e = thunderstorm;
            else if (event == "Snow") e = snow;
            
            p.events.push_back(e);
        }
        
        p.wind_direction = stoi(fix(line["WindDirDegrees"]));
        
        parsed.push_back(p);
    }
    
    return parsed;
}
