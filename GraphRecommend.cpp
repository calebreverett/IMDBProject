#include "GraphRecommend.h"

GraphRecommend::GraphRecommend(){
    readFileIntoGraph();
}

void GraphRecommend::readFileIntoGraph() {
    //Reads in all of the files
    ifstream file("../data/title.basics.tsv");
    string line;
    //Gets the title section of the file
    getline(file, line);
    string id;
    while(getline(file, id, '\t')){
        //stops the file read if it reaches the end(solves some bugs)
        if(id == "\n")
            break;
        string type;
        string name;
        string originalTitle;
        string isAdult;
        string startYear;
        string endYear;
        string runtime;
        string genres;

        getline(file, type, '\t');
        getline(file, name, '\t');
        getline(file, originalTitle, '\t');
        getline(file, isAdult, '\t');
        getline(file, startYear, '\t');
        getline(file, endYear, '\t');
        getline(file, runtime, '\t');
        getline(file, genres);

        //Does not include Adult Movies, Tv Episodes, or Videogames
        if(isAdult == "0" && type != "tvEpisode" && type != "videoGame"){
            Movie tempMovie;
            tempMovie.title = name;
            if (startYear != "\\N") {
                tempMovie.year = stoi(startYear);
            } else if (endYear != "\\N") {
                tempMovie.year = stoi(endYear);
            } else {
                tempMovie.year = 0;
            }

            tempMovie.originalTitle = originalTitle;
            tempMovie.titleType = type;

            //Reads in all genres which are seperated by commas
            for (int i = 0; i < genres.length(); i++) {
                string genre = "";
                if (genres.at(i) != ',') {
                    genre += genres.at(i);
                } else {
                    tempMovie.genres[genre] = true;
                    genre = "";
                }
            }

            //Adds the movie to the unordered_map
            movies[id] = tempMovie;
        }
    }
    file.close();


    file.open("../data/title.crew.tsv");
    getline(file, line);
    while(getline(file, id, '\t')){
        if(id == "\n")
            break;
        string directors;
        string writers;

        getline(file, directors, '\t');
        getline(file, writers);


        //Only adds directors and writers if they are some in the database
        if(directors != "\\N") {
            string tempDirector;
            stringstream directorStream(directors);
            //Reads in all directors which are seperated by commas
            while(getline(directorStream, tempDirector, ',')){
                movies[id].directors.push_back(tempDirector);
            }
        }
        if(writers != "\\N") {
            string tempWriter;
            stringstream writerStream(writers);
            while(getline(writerStream, tempWriter, ',')){
                movies[id].writers.push_back(tempWriter);
            }
        }
    }
    file.close();

    file.open("../data/title.ratings.tsv");
    getline(file, line);
    while(getline(file, id, '\t')){
        if(id == "\n")
            break;
        string rating;
        string numVotes;

        getline(file, rating, '\t');
        getline(file, numVotes);

        //Transforms the strings into the necessary data types
        movies[id].rating = stof(rating);
        movies[id].numVotes = stoi(numVotes);
    }
    file.close();
}

vector<string> GraphRecommend::recommendMovie(string movie){
    string movieID;
    vector<string> returnVector;
    //Finds the ID of the title (returns the first title that matches
    for(auto itr = movies.begin(); itr != movies.end(); itr++) {
        if (itr->second.title == movie || itr->second.originalTitle == movie) {
            movieID = itr->first;
            break;
        }
    }

    //If the ID exists continue with the recommendation
    if(movies.find(movieID) != movies.end()){
        //Makes sure a movie isn't added twice
        if(graph.find(movieID) == graph.end()) {
            //If the movie doesn't include any genres it links it to all other movies
            if (movies[movieID].genres.empty()) {
                for (auto itr = movies.begin(); itr != movies.end(); itr++)
                    //Gets Weights
                    graph[movieID][itr->first] = getWeight(movies[movieID], movies[itr->first]);
            }
            //If it has genres it is linked to every other movie in those genres
            else {
                for (auto itr = movies.begin(); itr != movies.end(); itr++) {
                    for (auto itr2 = movies[itr->first].genres.begin(); itr2 != movies[itr->first].genres.end(); itr2++) {
                        if (movies[itr->first].genres[itr2->first] && movies[movieID].genres[itr2->first] && graph.find(itr->first) == graph.end())
                            //Gets Weights
                            graph[movieID][itr->first] = getWeight(movies[movieID], movies[itr->first]);
                    }
                }
            }
        }

        //Finds the 5 max weights then prints it out
        priority_queue<int> maxes;
        for(auto itr = graph[movieID].begin(); itr != graph[movieID].end(); itr++){
            //Makes sure a movie doesn't point to itself
            if(itr->first != movieID)
                maxes.push(itr->second);
        }
        int max = maxes.top();
        //Gets the maxes for the return vector
        for(int i = 0; i < 5; i ++){
            string title = findMax(max, movieID, returnVector);
            returnVector.push_back(title);
            maxes.pop();
            max = maxes.top();
        }
    }

    //If there is no movie or Tv show returns an empty vector, else returns the vector
    return returnVector;
}

int GraphRecommend::getWeight(Movie movieFrom, Movie movieTo) {
    //Crafts the weights based on the different stats of the movies
    int weight = 0;

    if(movieFrom.year == movieTo.year)
        weight += 10;
    if(movieFrom.year / 10 == movieTo.year / 10)
        weight += 20;

    if(movieFrom.titleType == movieTo.titleType)
        weight += 100;

    if(movieTo.title.find(movieFrom.originalTitle) != string::npos)
        weight += 10;

    if(movieFrom.title.find(movieTo.originalTitle) != string::npos)
        weight += 10;

    for (int i = 0; i < movieFrom.directors.size(); i++) {
        for (int j = 0; j < movieTo.directors.size(); j++) {
            if (movieFrom.directors.at(i) == movieTo.directors.at(j))
                weight += 40;
        }
    }
    for (int i = 0; i < movieFrom.writers.size(); i++) {
        for (int j = 0; j < movieTo.writers.size(); j++) {
            if (movieFrom.writers.at(i) == movieTo.writers.at(j))
                weight += 25;
        }
    }

    if(int(movieFrom.rating) == int(movieTo.rating))
        weight += 20;

    if(int(movieFrom.numVotes / 1000) == int(movieTo.numVotes / 1000))
        weight += 20;


    return weight;
}

//Function to find the max in the list, because it will need to be called multipe times
string GraphRecommend::findMax(int max, string movieID, vector<string> maxMovies) {
    for(auto itr = graph[movieID].begin(); itr != graph[movieID].end(); itr++){
        if(itr->second == max){
            string title = movies[itr->first].title;

            bool isIncluded = false;
            //Fixes a bug when multiple movies have the same weight
            for(int i = 0; i < maxMovies.size(); i++){
                //Continues if the movie has already been included
                if(title == maxMovies[i]){
                    isIncluded = true;
                }
            }
            if(!isIncluded)
                return title;
        }
    }
    //If bug occurs returns nothing
    return "";
}