#pragma once

#include <string>
#include <vector>
#include <unordered_map>

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