#include "HashRecommend.h"

HashRecommend::HashRecommend() {
    //This is the exact code as the ReadFileToGraph function in GraphRecommend.h
    
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

vector<string> HashRecommend::Recommend(string title) {
    //This will replicate the movie recomendation algorithm from GraphRecommend as a hash;
    
    //Create the return vector
    vector<string> recomendations;

    //Seach code for the graph comes from GraphRecommend
    //Finds the ID of the title (returns the first title that matches)
    string movieID = "";
    for(auto itr = movies.begin(); itr != movies.end(); itr++) {
        if (itr->second.title == title || itr->second.originalTitle == title) {
            movieID = itr->first;
            break;
        }
    }

    //If the movie id is found, create the hash table
    if(movieID != "") {
        //Create the array that's going to store the values
        vector<string> hashTable[2001]; //2000 is the theoretical max weight to a movie recomendation + 1 = 2001
        //Iterate through the hash table, and instantiate the vectors
        for(int i = 0; i < 2000; i++) {
            hashTable[i] = vector<string>();
        }
        
        //Saved temp iterator for duplicate check
        auto dupCheck = movies.find(movieID);
        //Grab the movie from the map
        Movie selection = dupCheck->second;
        //Temp Movie for the iterator
        Movie temp;
        //Iterate through the map again, but this time, assign a hash value to each movie
        int hashValue = 0;
        for(auto iter = movies.begin(); iter != movies.end(); iter++) {
            //Check to see if it's not on the selected movie
            if(iter != dupCheck) {
                //Assign temp to the iterator's movie
                temp = iter->second;
                //Weights from GraphRecommend
                if(selection.year == temp.year) {
                    hashValue += 10;
                }

                if(selection.year / 10 == temp.year / 10) {
                    hashValue += 20;
                }
                
                if(selection.titleType == temp.titleType) {
                    hashValue += 100;
                }

                if(selection.title.find(temp.originalTitle) != string::npos) {
                    hashValue += 10;
                }

                if(temp.title.find(selection.originalTitle) != string::npos) {
                    hashValue += 10;
                }
                
                for(int i = 0; i < selection.directors.size(); i++) {
                    for(int j = 0; j < temp.directors.size(); j++) {
                        if(temp.directors.at(j) == selection.directors.at(i)) {
                            hashValue += 40;
                            break;
                        }
                    }
                }

                for(int i = 0; i < selection.writers.size(); i++) {
                    for(int j = 0; j < temp.writers.size(); j++) {
                        if(temp.writers.at(j) == selection.writers.at(i)) {
                            hashValue += 25;
                            break;
                        }
                    }
                }

                for(int i = 0; i < selection.genres.size(); i++) {
                    for(int j = 0; j < temp.genres.size(); j++) {
                        if(temp.directors.at(j) == selection.directors.at(i)) {
                            hashValue += 100;
                            break;
                        }
                    }
                }

                if(int(selection.rating) == int(temp.rating)) {
                    hashValue += 20;
                }

                if(int(selection.numVotes) / 1000 == int(selection.numVotes) / 1000) {
                    hashValue += 20;
                }

                //If the hash value is bigger than the index, set it to the max
                if(hashValue > 2000) hashValue = 2000;

                //Place the movie in the correct place
                hashTable[hashValue].push_back(temp.title);
            }
            
            //Iterate through the hash table, and grab the 5 highest scoring movies
            for(int i = 2000; i >= 0; i--) {
                if(recomendations.size() == 5) {
                    break;
                }
                
                if(!hashTable[i].empty()) {
                    for(int j = 0; j < hashTable[i].size(); j++) {
                        recomendations.push_back(hashTable[i].at(j));
                        if(recomendations.size() == 5) {
                            break;
                        }
                    }
                }
            }
        }
    }

    return recomendations;
}
