#include "test_performance.hpp"

vector<test_perf> make_tests_perf()
{
    vector<weather_point> weather_data = load_weather("../data/weather_kjfk_1948-2014.csv");

    vector<test_perf> tests;

    tests.push_back(test_perf("where() by temperature", 2000, [=]
    {
        cinq::from(weather_data)
              .where([](const auto& x) -> bool { return x.temp_max > 90; });
    }));

    tests.push_back(test_perf("where() by temperature - manual", 2000, [=]
    {
        vector<weather_point> result;
        for (const auto& data : weather_data)
        {
            if (data.temp_max > 90) result.push_back(data);
        }
    }));

    tests.push_back(test_perf("select() mapping weather_point to cloud_cover", 2000, [=]
    {
       cinq::from(weather_data).select([](const auto& x){return x.cloud_cover;});
    }));

     tests.push_back(test_perf("select() mapping weather_point to cloud_cover - manual", 2000, [=]
    {
        vector<int> result;
        for(auto& data:weather_data){
            result.push_back(data.cloud_cover);
        }

    }));

    tests.push_back(test_perf("where().average() finding the averge cloud_cover between 1980 and 2000", 500, [=]
    {

        cinq::from(weather_data).where([](const auto& wp){return (1980-1900 <wp.date.tm_year && wp.date.tm_year< 2000-1900);})
                                .average([](const auto& wp){return wp.cloud_cover;});

    }));

    tests.push_back(test_perf("where().average() finding the averge cloud_cover between 1980 and 2000 - manual", 500, [=]
    {
        vector<weather_point> result;
        for (auto& data : weather_data)
        {
            if (1980-1900 <data.date.tm_year && data.date.tm_year< 2000-1900) result.push_back(data);
        }
        double sum=0;
        for (auto& data: result){
                   sum+=data.cloud_cover;
        }
       //double avg = sum/result.size();


    }));

    tests.push_back(test_perf("max(). finding the max temp_max in the data set ", 130000000, [=]
    {
        cinq::from(weather_data).max([](const auto& x){return x.temp_max;});
     //   cout<<max<<endl;

    }));
    tests.push_back(test_perf("max(). finding the max temp_max in the data set - manual ", 130000000, [=]
    {

       int locmax =numeric_limits<int>::min();
       for(const auto& data:weather_data)
       {
            if(locmax<data.temp_max) locmax=data.temp_max;
       }
     //  cout<<locmax<<endl;

     }));

    tests.push_back(test_perf("min(). finding the min temp_min in the data set", 130000000, [=]
    {

        cinq::from(weather_data).min([](const auto& x){return x.temp_min;});


    }));

    tests.push_back(test_perf("where().select(). get a vector of temp_mins for the days that it snowed", 2000, [=]
    {
        cinq::from(weather_data).where([](const auto& x){return x.snow;}).select([](const auto& x){return x.temp_min;}).to_vector();
    }));

    tests.push_back(test_perf("where().select(). get a vector of temp_mins for the days that it snowed - manual", 2000, [=]
    {
        vector<weather_point> result;
        for(auto& data: weather_data){
           // cout<<data.snow<<endl;
          if(data.snow) result.push_back(data);

        }
        vector<int> temps;
        for(auto& data: result){
          temps.push_back(data.temp_min);
        }
       // cout<<temps.size()<<endl;

    }));

    tests.push_back(test_perf("where().select().order_by().take() - 5 coldest rainy days", 100, [=]
    {
        cinq::from(weather_data)
             .where([](const weather_point& w) { return w.rain; })
             .order_by([](const weather_point& w) { return w.temp_min; })
             .take(5)
             .select([](const weather_point& w) { return w.temp_min; })
             .to_vector();
    }));
    
    tests.push_back(test_perf("where().select().order_by().take() - 5 coldest rainy days - manual", 100, [=]
    {
        vector<weather_point> result;
        for (auto& data : weather_data)
        {
            if (data.rain) result.push_back(data);
        }
        
        sort(result.begin(), result.end(), [](const auto &a, const auto &b) { return a.temp_min < b.temp_min; });
        
        vector<weather_point> five;
        for (size_t i = 0; i < 5; i++) five.push_back(result[i]);
        
        vector<int> temps;
        for (auto& data : five) temps.push_back(data.temp_min);
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
        p.fog=false;
        p.rain=false;
        p.thunderstorm=false;
        p.snow=false;
        auto result = split(line["Events"], '-');
        for (string event : result)
        {
            if(event == "Fog") p.fog=true;
            if(event == "Rain") p.rain=true;
            if(event == "Thunderstorm") p.thunderstorm=true;  
            if(event == "Snow") p.snow=true;  
        }
       
        p.wind_direction = stoi(fix(line["WindDirDegrees"]));

        parsed.push_back(p);
    }

    return parsed;
}
