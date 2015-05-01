# CINQ: C++ Integrated Query

An implementation of Microsoft's [Language Integrated Query](https://en.wikipedia.org/wiki/Language_Integrated_Query) for C++.

This library is our class project for [Bjarne Stroustrup's](http://www.stroustrup.com/) Language Library Design class.

## Why LINQ?

LINQ allows you to simplify data processing code. For example, both snippets below do the same thing --- find the temperatures of the five coldest rainy days in the provided data set.

Normal version:

    vector<weather_point> result;
    for (auto& data : weather_data)
    {
         if (data.rain) result.push_back(data);
    }
            
    sort(result.begin(), result.end(),
        [](const auto &a, const auto &b) { return a.temp_min < b.temp_min; });
            
    vector<weather_point> five;
    for (size_t i = 0; i < 5; i++) five.push_back(result[i]);
            
    vector<int> temps;
    for (auto& data : five) temps.push_back(data.temp_min);

The LINQ version is much easier to understand:

    cinq::from(weather_data)
         .where([](const weather_point& w) { return w.rain; })
         .order_by([](const weather_point& w) { return w.temp_min; })
         .take(5)
         .select([](const weather_point& w) { return w.temp_min; })
         .to_vector();

Intrigued? Please take a look at our documentation linked below.

## Learn more

- [In-depth tutorial on using CINQ](https://github.com/jeb2239/CINQ/blob/master/docs/tutorial.md)
- [Design report](https://github.com/jeb2239/CINQ/blob/master/docs/design_document.md)
- [How to set up C++ concepts](https://github.com/jeb2239/CINQ/tree/master/docs/concepts_setup.md)

## Authors

- Kevin Chen [@kevin1](https://github.com/kevin1)
- Jonathan Barrios [@jeb2239](https://github.com/jeb2239)
- Jonathan Wong [@thejonathanwong](https://github.com/thejonathanwong)
