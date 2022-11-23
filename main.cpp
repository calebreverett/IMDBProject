#include <iostream>
#include "GraphRecommend.h"
using namespace std;

int main() {
    GraphRecommend graph;
    string movie;

    while(true) {
        cout << "What is your Favorite Movie or TV Series? (enter \"stop\" to exit) \n";
        getline(cin, movie);

        if(movie == "stop"){
            break;
        }

        string movieRec;
        graph.recommendMovie(movie);
    }

    //cout << movieRec;
}
