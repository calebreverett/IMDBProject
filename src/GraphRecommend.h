#ifndef PROJECT3_GRAPHRECOMMEND_H
#define PROJECT3_GRAPHRECOMMEND_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>

//The Movie Struct was extracted from this file into it's own file so that it can be used for HashRecommmend
#include "Movie.h"

using namespace std;

class GraphRecommend {
    //Graph structues
    unordered_map<string, unordered_map<string, int>> graph;
    unordered_map<string, Movie> movies;

public:
    GraphRecommend();
    //Helper functions
    void readFileIntoGraph();
    int getWeight(Movie movieFrom, Movie movieTo);
    vector<string> recommendMovie(string movie);
    string findMax(int max, string movieID, vector<string> maxMovies);
};


#endif //PROJECT3_GRAPHRECOMMEND_H
