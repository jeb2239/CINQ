#include "test_performance.hpp"

vector<test_perf> make_tests_perf()
{
    vector<weather_point> weather_data = load_weather("../data/weather_kjfk_1948-2014.csv");
    
    vector<test_perf> tests;
    
    tests.push_back(test_perf("my great test", []
    {
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

vector<weather_point> load_weather(string path)
{
    ifstream source(path, ios::in);
    vector<weather_point> parsed;
    
    string tmp;
    getline(source, tmp);
    vector<string> headers = split(tmp, ',');
    
    int n = 0;
    while (getline(source, tmp))
    {
        // Get a line and put it in a dictionary
        vector<string> cells = split(tmp, ',');
        unordered_map<string, string> line;
        if (cells.size() != headers.size()) throw invalid_argument("invalid csv file");
        for (size_t i = 0; i < cells.size(); i++)
        {
            line[headers[i]] = cells[i];
            printf("%s = %s\n", headers[i].c_str(), cells[i].c_str());
        }
        printf("\n");
        if (n++ > 5) break;
    }
    
    return parsed;
}
