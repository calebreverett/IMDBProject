#ifndef PROJECT3_GRAPHRECOMMEND_H
#define PROJECT3_GRAPHRECOMMEND_H

#include <vector>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

//Creates a Movie struct to store all the movie info
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
    //Graph structues
    unordered_map<string, unordered_map<string, int>> graph;
    unordered_map<string, Movie> movies;

public:
    GraphRecommend();
    //Helper functions
    void readFileIntoGraph();
    int getWeight(Movie movieFrom, Movie movieTo);
    string recommendMovie(string movie);
};


#endif //PROJECT3_GRAPHRECOMMEND_H
