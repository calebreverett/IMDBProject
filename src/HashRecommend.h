#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <map>

#include "Movie.h"

class HashRecommend {
public:
    HashRecommend();
    vector<string> Recommend(string title);
private:
    map<string, Movie> movies;
};