#include <iostream>
#include "GraphRecommend.h"
using namespace std;

int main() {
    //Creates graph object (takes a few seconds due to size of dataset)
    GraphRecommend graph;
    string movie;

    //While loop in order to get multiple recommendations
    while(true) {
        //Prompt
        cout << "What is your Favorite Movie or TV Series? (enter \"stop\" to exit) \n";
        getline(cin, movie);

        //Exits loop with keyword
        if(movie == "stop"){
            break;
        }

        string movieRec;
        //Gets the movie rec (takes a few seconds due to size of dataset)
        movieRec = graph.recommendMovie(movie);

        //Prints the necessary information (May be changed with the UI integration)
        if(!movieRec.empty())
            cout << "Your recommendation is: " << movieRec << "\n";
        else
            cout << "This movie or show is not included in this streaming package. Please select another movie or show. \n";
    }

}
