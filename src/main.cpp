#include <iostream>
#include "GraphRecommend.h"
#include "HashRecommend.h"
using namespace std;

int main() {
    //Creates graph object (takes a few seconds due to size of dataset)
    GraphRecommend graph;
    //Creates hash object (takes about the same time as graph to make)
    HashRecommend hash;
    string movie;

    //Ask for graph or hash structure
    cout << "Would you like to use a graph or hash structure (type \"graph\" for a graph, and \"hash\" for a hash)" << '\n';
    string choice;
    getline(cin, choice);

    //use a boolean to detect if it's a graph or hash
    bool isHash;
    if (choice == "hash") {
        cout << "You have chosen hash" << '\n';
        isHash = true;
    }
    else {
        cout << "You have chosen graph" << '\n';
        isHash = false;
    }

    //While loop in order to get multiple recommendations
    while (true) {
        //Prompt
        cout << "What is your Favorite Movie or TV Series? (enter \"stop\" to exit)\n";
        getline(cin, movie);

        //Exits loop with keyword
        if (movie == "stop") {
            break;
        }
        if (movie == "\n" || movie == "") {//Fixes an error which returns nothing when entered nothing
            cout << "This movie or show is not included in this streaming package. Please select another movie or show. \n";
            continue;
        }

        vector<string> movieRecs;
        //Gets the movie rec (takes a few seconds due to size of dataset)
        if (!isHash) movieRecs = graph.recommendMovie(movie);
        else movieRecs = hash.Recommend(movie);

        //Prints the necessary information (May be changed with the UI integration)
        if (!movieRecs.empty())
            for (int i = 0; i < movieRecs.size(); i++)
                cout << "Your recommendation is: " << movieRecs[i] << "\n";
        else
            cout << "This movie or show is not included in this streaming package. Please select another movie or show. \n";
    }

}
