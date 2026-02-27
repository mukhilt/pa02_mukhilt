#ifndef MOVIES_H
#define MOVIES_H
#include <string>
using namespace std;

class Movie {
    public:
        string name;
        double rating;
        Movie(string n, double r) {
            name = n;
            rating = r;
        }
};
#endif