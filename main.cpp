// Winter'24
// Instructor: Diba Mirza
// Student name: Mukhil Thavathiru Murugan
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <vector>
#include <cstring>
#include <algorithm>
#include <limits.h>
#include <iomanip>
#include <set>
#include <queue>
#include <map>
#include <sstream>
using namespace std;

#include "utilities.h"
#include "movies.h"

bool parseLine(string &line, string &movieName, double &movieRating);
bool comparison(pair<string, double> p1, pair<string, double> p2) {
    if (p1.second != p2.second) {
        return p1.second > p2.second;
    }
    else {
        return p1.first < p2.first;
    }
}

int main(int argc, char** argv){
    if (argc < 2){
        cerr << "Not enough arguments provided (need at least 1 argument)." << endl;
        cerr << "Usage: " << argv[ 0 ] << " moviesFilename prefixFilename " << endl;
        exit(1);
    }

    ifstream movieFile (argv[1]);
 
    if (movieFile.fail()){
        cerr << "Could not open file " << argv[1];
        exit(1);
    }
  
    map<string, double> movies;

    string line, movieName;
    double movieRating;
    // Read each file and store the name and rating
    while (getline (movieFile, line) && parseLine(line, movieName, movieRating)){
        Movie m(movieName, movieRating);
        movies[movieName] = movieRating;
            // Use std::string movieName and double movieRating
            // to construct your Movie objects
            // cout << movieName << " has rating " << movieRating << endl;
            // insert elements into your data structure
    }

    movieFile.close();

    if (argc == 2){
        for (auto it = movies.begin(); it != movies.end(); ++it) {
            cout << it->first << ", " << fixed << setprecision(1) << it->second << endl;
        }
            return 0;
    }

    ifstream prefixFile (argv[2]);

    if (prefixFile.fail()) {
        cerr << "Could not open file " << argv[2];
        exit(1);
    }

    vector<string> prefixes;
    while (getline (prefixFile, line)) {
        if (!line.empty()) {
            prefixes.push_back(line);
        }
    }
    vector<pair<string, pair<string,double>>> bests;
    for (auto it = prefixes.begin(); it != prefixes.end(); ++it) {
        string prefix = *it;
        vector<pair<string, double>> moviesVec;
        for (auto itT = movies.begin(); itT != movies.end(); ++itT) {
            string theName = itT->first;
            if (theName.substr(0,prefix.length()) == prefix) {
                moviesVec.push_back({itT->first, itT->second});
            }
        }
        if (moviesVec.empty()) {
            cout << "No movies found with prefix " << prefix << endl;
        }
        else{
            sort(moviesVec.begin(), moviesVec.end(), comparison);
            for (auto itTT = moviesVec.begin(); itTT != moviesVec.end(); ++itTT) {
                cout << itTT->first << ", " << fixed << setprecision(1) << itTT->second << endl;
            }
            cout << endl;
            bests.push_back({prefix, {moviesVec[0].first, moviesVec[0].second}});
        }
    }
    for (int i = 0; i < bests.size(); i++) {
        cout << "Best movie with prefix " << bests[i].first << " is " << bests[i].second.first << " with rating " << fixed << setprecision(1) << bests[i].second.second << endl;
    }

    //  For each prefix,
    //  Find all movies that have that prefix and store them in an appropriate data structure
    //  If no movie with that prefix exists print the following message
    //cout << "No movies found with prefix "<<"<replace with prefix>" << endl;

    //  For each prefix,
    //  Print the highest rated movie with that prefix if it exists.
    //cout << "Best movie with prefix " << "<replace with prefix>" << " is: " << "replace with movie name" << " with rating " << std::fixed << std::setprecision(1) << "replace with movie rating" << endl;

    return 0;
}

//3A
//Our main structure we used here was the order map for storing the movies. The average cost for an ordered mao is O(log n)
//And because we are inserting n number of movies, that left us with a time complexity of O(nlog n). Next up, we look through
//every single movie. This leads to a time complexity of O(n). However, we repeat this process every time for m prefixes. Thus
//out time complexity for scanning gives us O(mn). During our sort, our time should cost log k. And because we are sorting k (movies with the prefix),
//the overall cost for storing will be O(klog k) per prefix. We again repeat the process. This time, we repeat for the movies
//that start with the prefix. Therefore, the overall complexity will be O(mklog k). Finally, our overarching logic when it comes to
//checking prefix is that we run through all n movies for each m prefix. This leaves us with a time complexity of O(mn).
//In our print statement, we run through k matching movies k times for m prefixes. This leaves us with a time complexity
//of O(km). When we put all of these together, we get an overall complexity of: O(nlogn + mn + mklog k + mk). The worst-case time complexity,
//assuming that k can be n is O(mnlog n). However, randomized data sets would lead to the dominant time complexity to be O(mn).

//20 input: 0.032 (32ms)
//100 input: 0.276 (276ms)
//1000 input: 0.254 (254ms)
//76920 input: 33.221 (33221ms)

//3B
//For the space complexity, we can first look at the map that we created. In the map, we store n number of movie titles
//with each having l number of max characters. This will leave us with a space complexity og O(nl). Secondly, we have a
//vector that stores m number of prefixes, each has a max l number of characters. This results in a space complexity of O(ml).
//Finally, we have another data structure, the vector pair. This structure is made to hold k number of matches, which again holds
//l max characters. Thus a resulting in a space complexity of O(kl). When we put all of this together, we get an overall
//complexity of O(nl + ml + kl). The dominant term in this will be O(nl).

//3C
//My thought process was to implement a simplistic process with the intentions of keeping a low space complexity. To maintain
//simplicity, I chose to implement an ordered map to store the movies and ratings. This resulted in a space complexity of O(nl)
//Additionally, I used a vector to store all the matching movies. This results in a space complexity of O(k). However, that being said,
//I did not achieve a very low time complexity. The main reason why I wasn't able to achieve a very low time complexity
//is because every movie is being traversed for every prefix.

/* Add your run time analysis for part 3 of the assignment here as commented block*/

bool parseLine(string &line, string &movieName, double &movieRating) {
    int commaIndex = line.find_last_of(",");
    movieName = line.substr(0, commaIndex);
    movieRating = stod(line.substr(commaIndex+1));
    if (movieName[0] == '\"') {
        movieName = movieName.substr(1, movieName.length() - 2);
    }
    return true;
}
