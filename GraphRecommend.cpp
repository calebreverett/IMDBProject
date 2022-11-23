#include "GraphRecommend.h"

GraphRecommend::GraphRecommend(){
    readFileIntoGraph();
}

void GraphRecommend::readFileIntoGraph() {
    ifstream file("../data/title.basics.tsv");
    string line;
    getline(file, line);

    string id;
    while(getline(file, id, '\t')){
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
            for (int i = 0; i < genres.length(); i++) {
                string genre = "";
                if (genres.at(i) != ',') {
                    genre += genres.at(i);
                } else {
                    tempMovie.genres[genre] = true;
                    genre = "";
                }
            }

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


        if(directors != "\\N") {
            string tempDirector;
            stringstream directorStream(directors);
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

        movies[id].rating = stof(rating);
        movies[id].numVotes = stoi(numVotes);
    }

    file.close();

}

string GraphRecommend::recommendMovie(string movie){
    string movieID;
    for(auto itr = movies.begin(); itr != movies.end(); itr++) {
        if (itr->second.title == movie || itr->second.originalTitle == movie) {
            movieID = itr->first;
            break;
        }
    }

    if(movies.find(movieID) != movies.end()){
        if(graph.find(movieID) == graph.end()) {
            if (movies[movieID].genres.empty()) {
                for (auto itr = movies.begin(); itr != movies.end(); itr++) {
                    graph[movieID][itr->first] = getWeight(movies[movieID], movies[itr->first]);
                }
            } else {
                for (auto itr = movies.begin(); itr != movies.end(); itr++) {
                    for (auto itr2 = movies[itr->first].genres.begin();
                         itr2 != movies[itr->first].genres.end(); itr2++) {
                        if (movies[itr->first].genres[itr2->first] && movies[movieID].genres[itr2->first] &&
                            graph.find(itr->first) == graph.end()) {
                            graph[movieID][itr->first] = getWeight(movies[movieID], movies[itr->first]);
                        }
                    }
                }
            }
        }

        int max = 0;
        for(auto itr = graph[movieID].begin(); itr != graph[movieID].end(); itr++){
            if(itr->second > max && itr->first != movieID){
                max = itr->second;
            }
        }
        for(auto itr = graph[movieID].begin(); itr != graph[movieID].end(); itr++){
            if(itr->second == max){
                string title = movies[itr->first].title;
                cout << "Your recommendation is: " << title << "\n";
                return itr->first;
            }
        }
    }
    else{
        cout << "This movie or show is not included in this streaming package. Please select another movie or show. \n";
    }

    return "";
}

int GraphRecommend::getWeight(Movie movieFrom, Movie movieTo) {
    int weight = 0;

    if(movieFrom.year == movieTo.year)
        weight += 20;
    if(movieFrom.year / 10 == movieTo.year / 10)
        weight += 10;

    if(movieFrom.titleType == movieTo.titleType)
        weight += 100;

    for (int i = 0; i < movieFrom.directors.size(); i++) {
        for (int j = 0; j < movieTo.directors.size(); j++) {
            if (movieFrom.directors.at(i) == movieTo.directors.at(j))
                weight += 100;
        }
    }
    for (int i = 0; i < movieFrom.writers.size(); i++) {
        for (int j = 0; j < movieTo.writers.size(); j++) {
            if (movieFrom.writers.at(i) == movieTo.writers.at(j))
                weight += 25;
        }
    }

    if(int(movieFrom.rating) == int(movieTo.rating))
        weight += 40;

    if(int(movieFrom.numVotes / 10) == int(movieTo.numVotes / 10))
        weight += 30;


    return weight;
}