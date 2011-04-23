///
/// @brief  Problem_A.cpp
/// @author Dohyun Yun ( dualistmage@wisenut.co.kr )
/// @date   2011.04.23
///

#include <iostream>
#include <fstream>
#include <queue>
#include <list>
#include <math.h>
#include <set>

using namespace std;

float abs(float num) { return (num > 0.0)?num:(-num); }

// ------------------------------------------------------------------ [ Class Definition ]
class RatingUnit
{
public:
    RatingUnit(int uid, int mid, float r) : userid_(uid), movieid_(mid), rating_(r) {};
    int userid_;
    int movieid_;
    float rating_;
}; // class RatingUnit

class ItemSimilarity
{
public:
    ItemSimilarity(int mid, float sim) : movieid_(mid), sim_(sim) {};
    bool operator< (const ItemSimilarity& rhs) const { return this->sim_ < rhs.sim_; }
    int movieid_;
    float sim_;
}; // class RatingUnit

// ------------------------------------------------------------------ [ Global variables ]
int K = 30;
int maxUser, maxMovie;
float** uiMat; // [userid][movieid]
float** simMat; // [movieid][movieid]
float* userCount;
float* movieCount;
set<int> userList;
set<int> movieList;

priority_queue<ItemSimilarity>* neighborList; // [movieid] // top K 

// ------------------------------------------------------------------ [ Function Definition ]
float getAdjustSimilarity(int movie2, int movie2);
float predictRating(int user, int movie, priority_queue<ItemSimilarity>* simMovieList);


///////////////////////////////////////////////////////////////////////////////////////////////////
// Main
///////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
    if ( argc < 3 )
    {
        cout << "[Usage] Problem_A <input> <output>" << endl;
        return -1;
    }


    ifstream fpin(argv[1]);
    ofstream fpout(argv[2]);
    if ( !fpin.is_open() )
    {
        cout << "input file(" << argv[1] << ") is not opened" << endl;
        return -2;
    }
    if ( !fpout.is_open() )
    {
        cout << "output file(" << argv[1] << ") is not opened" << endl;
        return -2;
    }

    int i,j;
    int userid, movieid, rating;
    list<RatingUnit> ratingList;

    maxUser = maxMovie = 0;

    // Find max user
    while( fpin.good() )
    {
        fpin >> userid >> movieid >> rating;
        if ( !fpin.good() )
            break;

        userList.insert(userid);
        movieList.insert(movieid);
        maxUser  = max(maxUser,userid);
        maxMovie = max(maxMovie,movieid);
        ratingList.push_back( RatingUnit(userid, movieid, rating) );

    } // end - while()

    maxUser++;
    maxMovie++;

    // Allocate ui matrix
    uiMat = new float*[maxUser + 1]; // uiMat[maxUser][maxItem] is average room for each user or item.
    userCount = new float[maxUser + 1]; // count is for calculating average

    for(i = 1; i < maxUser; i++)
    {
        uiMat[i] = new float[maxMovie + 1];
        for(j = 1; j < maxMovie; j++)
            uiMat[i][j] = 0.0;

        userCount[i] = 0;
    }

    // Assign values to uiMat, calculate average of user and movie
    list<RatingUnit>::iterator iter = ratingList.begin();
    for(; iter != ratingList.end(); iter++)
    {
        userid = iter->userid_;
        movieid = iter->movieid_;
        rating = static_cast<int>(iter->rating_);
        uiMat[userid][movieid] = rating;

        uiMat[userid][maxMovie] += rating;
        userCount[userid]++;
    }

    for(i = 1; i < maxUser; i++)
        uiMat[i][maxMovie] /= static_cast<float>(userCount[i]);

    // Calculating similarity for each item. its simmetry value. first index must be less than second index.
    neighborList = new priority_queue<ItemSimilarity>[maxMovie];
    
    for(i = 1; i < maxMovie; i++)
    {
        for(j = i + 1; j < maxMovie; j++)
        {
            float sim = getAdjustSimilarity(i,j);
            neighborList[i].push( ItemSimilarity(j,sim) );
            neighborList[j].push( ItemSimilarity(i,sim) );
        }
    }

    // Calculating top k neighbors for each item.
    int topk = 10;
    float pui;

    set<int>::iterator userIter = userList.begin();
    set<int>::iterator movieIter = movieList.begin();

    for(; userIter != userList.end(); userIter++)
    {
        priority_queue<ItemSimilarity> recommendedMovieList;
        for(; movieIter != movieList.end(); movieIter++)
        {
            pui = predictRating( *userIter, *movieIter, neighborList );
            recommendedMovieList.push( ItemSimilarity( *movieIter, pui ) );
        }

        // Output
        fpout << *userIter;
        size_t rMovieSize = recommendedMovieList.size();
        for(size_t i = 0; i < rMovieSize&& i < topk; i++)
        {
            fpout << "\t" << recommendedMovieList.top().movieid_;
            recommendedMovieList.pop();
        }
        fpout << endl;
    }

    fpin.close();
    fpout.close();

} // end - int main()




// ------------------------------------------------------------------ [ Function Implementation ]
float getAdjustSimilarity(int movie1, int movie2)
{
    float ave;
    float numerator = 0.0, root1 = 0.0, root2 = 0.0;
    float rui, ruj;
    int i,j;
    for(i = 1; i < maxUser; i++)
    {
        rui = uiMat[i][movie1] - ave;
        ruj = uiMat[i][movie2] - ave;

        if ( rui == 0.0 || ruj == 0.0 )
            continue;

        ave = uiMat[i][maxMovie];
        numerator += (rui - ave) * (ruj -ave);
        root1 += rui * rui;
        root2 += ruj * ruj;
    }

    float root = sqrt(root1) * sqrt(root2);
    if ( root == 0.0 )
        return 0.0;

    return numerator / root;
}

float predictRating(int user, int movie, priority_queue<ItemSimilarity>* simMovieList)
{
    int count = 0;
    int neighborMovie;
    float numerator = 0.0, root = 0.0, sim;
    float ramda = 0.7;

    priority_queue<ItemSimilarity> neighborMovieList(simMovieList[movie]);

    if ( neighborMovieList.size() == 0 )
        return ramda * uiMat[user][maxMovie] + (1-ramda)*uiMat[maxUser][movie];

    while( count < K && neighborMovieList.size() > 0 )
    {
        const ItemSimilarity& neighbor( neighborMovieList.top() );
        neighborMovie = neighbor.movieid_;
        sim = simMat[min(movie, neighborMovie)][max(movie,neighborMovie)];
        numerator += sim * uiMat[user][movie];
        root += abs(sim);
    }

    if ( root == 0.0 )
        return 0.0;

    return numerator / root;
}
