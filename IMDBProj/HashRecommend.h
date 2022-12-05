#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <map>

#include "Movie.h"

using namespace std;



class HashRecommend {
private:
    map<string, Movie> movies;
public:
    HashRecommend();
    vector<string> Recommend(string title);
};