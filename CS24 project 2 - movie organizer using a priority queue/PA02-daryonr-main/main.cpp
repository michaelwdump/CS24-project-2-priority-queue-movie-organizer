    /// michael wong 620928-
// daryon roshanzaer 5701388
#include "movies.h"
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
using namespace std;

/*
int main() {
    Movies a;
    a.print();
    return 0;
}
*/


bool parseLine(string &line, string &movieName, double &movieRating);

int main(int argc, char** argv){
    if (argc < 2){
        cerr << "Not enough arguments provided (need at least 1 argument)." << endl;
        cerr << "Usage: " << argv[ 0 ] << " filename prefix1 prefix2 ... prefix n " << endl;
        exit(1);
    }

    ifstream movieFile (argv[1]);
 
    if (movieFile.fail()){
        cerr << "Could not open file " << argv[1];
        exit(1);
    }
  
    // Create an object of a STL data-structure to store all the movies

    string line, movieName, movName, movRating;
    double movieRating, movRatingDouble;
    char quotes = '"';
    
    // Read each file and store the name and rating
    priority_queue<Movies, vector<Movies>, alphaOrder> alpha_pq;
    priority_queue<Movies, vector<Movies>, numOrder> rating_pq;


    //MICHAEL: this chunk of text goes thru every line the text file, sets a temp Movies object with a name and rating, then pushes that object to the alpha_pq    
    while (getline (movieFile, line) && parseLine(line, movieName, movieRating)){
            if (line.at(0) != quotes) { //no comma in movie name
                movName = line.substr(0, line.find(","));

                movRating = line.substr((line.find(",") + 1), line.length());
            }
            else { //commma in movie name, the movie is in quotations
                movName = line.substr(1, line.find_last_of("\"") - 1);
                movRating = line.substr((line.find_last_of("\"") + 2), line.length());
            }
        movRatingDouble = stod(movRating);
        Movies allMovies(movName, movRatingDouble);
        alpha_pq.push(allMovies);
    }

    movieFile.close();

    //step 1
    //MICHAEL: if u only have the txt file as an arg, then the programm will cout all the movies in alpha order
    if (argc == 2){
        //print all the movies in ascending alphabetical order of movie names
        
        while (!alpha_pq.empty()) {
            cout << alpha_pq.top();
            alpha_pq.pop();
        }
        
            return 0;
    }

    //step 2
    //MICHAEL: if u have 2+ args, the program will loop thru every additional arg. 
    //in every loop, the program will loop thru every movie in a copy of alpha_pq, and push that to the rating_pq if that movie has a user-specified prefix
    //
    if (argc > 2) {
       bool printLine = false;
	  //vectors to hold prefixes with no movies, and top movies of prefixe movies
        vector<bool>noPrefix;                                                        //SC: k
        vector<Movies>topMovie;                                                      //SC: k
        for (int i=2; i < argc; ++i ) {                                            //m
            //creating copy of alpha_pq
	    priority_queue<Movies, vector<Movies>, alphaOrder> alpha_pqCopy = alpha_pq; //n     //SC: N
            //cout << "currently on int " << i << ": " << argv[i] << endl;
            bool temp;
            //loop to push prefixed movies
            while (!alpha_pqCopy.empty()) {                                       //n*
                string movieHolder = (alpha_pqCopy.top()).getMovie();
                //string prefix = movieHolder.substr(0, movieHolder.find(" "));
                if (movieHolder.find(argv[i]) == 0) {                                 //k
                    rating_pq.push(alpha_pqCopy.top());                          //logk      //SC: k
                }
                alpha_pqCopy.pop();                                              //logk
            }

            //MICHAEL:: if/else branch that checks if a prefix has no movies. if it does have movies, then the else branch will 
            if (rating_pq.empty()) {                                            
                noPrefix.push_back(true);                                       //c 
                topMovie.push_back(Movies());                                   //c
                temp = false;
            }
            else {
                noPrefix.push_back(false);
                topMovie.push_back(rating_pq.top());
                temp = true;
            }

            //loop to cout every elem in rating_pq in decreasing rating order
            while (!rating_pq.empty()) {                                        //n
                cout << rating_pq.top();                                        //c
                rating_pq.pop();                                                //logn
            }
             
            if (temp) cout << endl;
        }

    //MICHAEL: this is a loop that checks a vector with true/false values if every prefix has no movies of that prefix or not
    for (int i=2; i < argc; ++i ) {
        if (noPrefix.at(i-2) == true) {
                cout << "No movies found with prefix " << argv[i] << endl;
		cout << endl;
        }
    }

    //  For each prefix,
    //  Print the highest rated movie with that prefix if it exists.
    for (int i=2; i < argc; ++i ) {
        if (topMovie.at(i-2).getMovie() != "NULLMOVIE") {
                cout << "Best movie with prefix " << argv[i] << " is: " << (topMovie.at(i-2)).getMovie() << " with rating " <<  (topMovie.at(i-2)).getRating() << endl;
        }
    }
  return 0;
}
}

/*
(step 0) The first while loop which reads in the movies and puts them into a priority queue. Overall, this has a O(n log n). This is because the while loop will have a worst case operating time of n times and the insert function of the priority queue will have an worst case operating time of O(log n). Multiplying these two together gives us the overall Big-O. 

Step 1 has a Big-O complexity of O(n log n). This is because there is a while loop which will iterate n times (O(n)). Within the while loop is a top(), O(1), and a pop() function, O(log n). Adding these two together and multiplying it by O(n) gives us the overall worst case running time of O(n log n). 

Step 2 has a Big-O of O(m n log k). This is because the beginning for loop will iterate for as many prefixes that are inputted (O(m)). Within the for loop is a while loop which will iterate throughout the entire movies list looking for a prefix match (O(n)). If there is a prefix match, there is a push() function O(log k) which will add it to a separate list. Multiplying these Big-Os together gives us an overall time complexity of O(m n log k).

Part b
Within the program there was a lot of additional space allocated as we needed multiple priority queues to sort and organize the data in addition to a container for the prefixes. For example, we needed separate priority queues to sort by rating (O(n)), alphabetical (O(n)) order, and prefix (O(m)). By combining together all the space complexities and reducing the coefficients, we get an overall space complexity of O(n + m). 

Part c
Our priority was a low time complexity with minor regard to space complexity. We simply wanted output speed to be quick. What ended up happening however was the opposite: we had an algorithm with a subpar time complexity but a good space complexity. Our high level implementation of our algorithm mentioned creating a priority queue with all the movies of the user-defined prefixes and then having a loop to cout all the movies in decreasing numerical order. It was estimated that the priority queue would take O(n) to make, and the loop to take o(logk) to run, therefore making a total time complexity of O(logk). However, we failed to consider how to loop through the m amount of prefixes and how exactly to fill in our priority queue, which contributed to a higher runtime than expected. These fine details in high level implementation weren’t accounted for, which led to a higher-than-expected time complexity.
*/

bool parseLine(string &line, string &movieName, double &movieRating) {
    if (line.length() <= 0) return false;
    string tempRating = "";

    bool flag = false;
    movieName = tempRating = "", movieRating = 0.0, flag = false;

    for (int i = 0; i < line.length(); i++){
        if(flag) tempRating += line[i];
        else if(line[i]==','&& line[0]!='"') flag = true;
        else {
            if(i==0 && line[0]=='"') continue;
            if(line[i]=='"'){ i++; flag=true; continue;}
            movieName += line[i];
        }
    }
    
    movieRating = stod(tempRating);
    return true;
}
