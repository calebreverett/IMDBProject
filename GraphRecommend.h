#ifndef PROJECT3_GRAPHRECOMMEND_H
#define PROJECT3_GRAPHRECOMMEND_H

#include <vector>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

struct Movie{

    string title;
    string titleType;
    string originalTitle;
    float rating = 0.0;
    int numVotes = 0;
    int year = 0;
    unordered_map<string, bool> genres;
    vector<string> writers;
    vector<string> directors;
};

class GraphRecommend {
    unordered_map<string, unordered_map<string, int>> graph;
    unordered_map<string, Movie> movies;

public:
    GraphRecommend();
    void readFileIntoGraph();
    void createGraph();
    int getWeight(Movie movieFrom, Movie movieTo);
    string recommendMovie(string movie);
};


#endif //PROJECT3_GRAPHRECOMMEND_H
