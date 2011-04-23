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
float* userCount;
float* movieCount;
set<int> userList;
set<int> movieList;

priority_queue<ItemSimilarity>* neighborList; // [movieid] // top K 

// ------------------------------------------------------------------ [ Function Definition ]
float getAdjustSimilarity(int movie1, int movie2); // movie1 must be smaller than movie2
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

    // DEBUG
    ofstream simOut("sim.out");
    ofstream itemOut("item.out");


    if ( !fpin.is_open() )
    {
        cout << "input file(" << argv[1] << ") is not opened" << endl;
        return -2;
    }
    if ( !fpout.is_open() )
    {
        cout << "output file(" << argv[2] << ") is not opened" << endl;
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

    // increase max user and movie because of array index
    maxUser++; 
    maxMovie++;

    set<int>::iterator userIter, movieIter, movieIter2;

    // Allocate ui matrix
    uiMat = new float*[maxUser + 1]; // uiMat[maxUser][maxItem] is average room for each user or item.
    userCount = new float[maxUser + 1]; // count is for calculating average
    movieCount = new float[maxMovie + 1]; // count is for calculating average

    for(userIter = userList.begin(); userIter != userList.end(); userIter++)
    {
        i = *userIter;
        uiMat[i] = new float[maxMovie + 1];
        for(movieIter = movieList.begin(); movieIter != movieList.end(); movieIter++)
            uiMat[i][*movieIter] = 0.0;
        userCount[i] = 0.0;
    }

    uiMat[maxUser] = new float[maxMovie]; // for Movie Average
    for(movieIter = movieList.begin(); movieIter != movieList.end(); movieIter++)
    {
        j = *movieIter;
        uiMat[maxUser][j] = 0.0;
        movieCount[j] = 0.0;
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

        uiMat[maxUser][movieid] += rating;
        movieCount[movieid]++;
    }
    for(userIter = userList.begin(); userIter != userList.end(); userIter++)
        uiMat[*userIter][maxMovie] /= static_cast<float>(userCount[*userIter]);
    for(movieIter = movieList.begin(); movieIter != movieList.end(); movieIter++)
        uiMat[maxUser][*movieIter] /= static_cast<float>(movieCount[*movieIter]);

    // Calculating similarity for each movie. The matrix is symmetry.
    cout << "Start calculating similarity" << endl; // DEBUG
    neighborList = new priority_queue<ItemSimilarity>[maxMovie];
    for(movieIter = movieList.begin(); movieIter != movieList.end(); movieIter++)
    {
        movieIter2 = movieIter; movieIter2++;
        for(; movieIter2 != movieList.end(); movieIter2++)
        {
            i = *movieIter; j = *movieIter2;
            float sim = getAdjustSimilarity(i,j);
            if ( sim <= 0.0 )
                continue;
            simOut<< i << "\t,\t" << j << "\t" << sim << endl;
            neighborList[i].push( ItemSimilarity(j,sim) );
            neighborList[j].push( ItemSimilarity(i,sim) );
        }
    }
    simOut.close();

    //DEBUG
    for(movieIter = movieList.begin(); movieIter != movieList.end(); movieIter++)
    {
        i = *movieIter;
        priority_queue<ItemSimilarity> neighborMovieList(neighborList[i]);
        itemOut << "[\t" << i << "]\t" << movieCount[i] << ":\t" << uiMat[maxUser][i] << "[\t" << neighborMovieList.size() << "\t->";
        while( neighborMovieList.size() > 0 )
        {
            itemOut << "\t" << neighborMovieList.top().movieid_;
            neighborMovieList.pop();
        }
        itemOut << endl;
    }
    itemOut.close();
    //DEBUG
    


//     // DEBUG
//     movieIter = movieList.begin();
//     for(; movieIter != movieList.end(); movieIter++)
//     {
//         int count = 0;
//         cerr << *movieIter;
//         priority_queue<ItemSimilarity>& neighborMovieList(neighborList[*movieIter]);
// 
//         while( count < K && neighborMovieList.size() > 0 )
//         {
//             cerr << "\t" << neighborMovieList.top().movieid_;
//             count++;
//             neighborMovieList.pop();
//         }
//         cerr << endl;
//     }
//     // DEBUG



    // Calculating top k neighbors for each item.
    float pui;
    size_t topk = 10;

    for(userIter = userList.begin(); userIter != userList.end(); userIter++)
    {
        cout << "Calculate user : " << *userIter << endl; // DEBUG
        i = *userIter;
        priority_queue<ItemSimilarity> recommendedMovieList;
        for(movieIter = movieList.begin(); movieIter != movieList.end(); movieIter++)
        {
            j = *movieIter;
            pui = uiMat[i][j];
            if ( pui == 0.0 )
                pui = predictRating( i, j, neighborList );
            recommendedMovieList.push( ItemSimilarity( j, pui ) );
        }

        // Output
        fpout << i;
        size_t rMovieSize = recommendedMovieList.size();
        for(size_t i = 0; i < rMovieSize && i < topk; i++)
        {
            fpout << "\t" << recommendedMovieList.top().movieid_ << "(" << recommendedMovieList.top().sim_ << ")";
            recommendedMovieList.pop();
        }
        fpout << endl;
    }
    fpin.close();
    fpout.close();


} // end - int main()




// ------------------------------------------------------------------ [ Function Implementation ]
float getAdjustSimilarity(int i, int j)
{
    float ave;
    float numerator = 0.0, root1 = 0.0, root2 = 0.0;
    float rui, ruj;
    int u;
    set<int>::iterator userIter = userList.begin();
    for(; userIter != userList.end(); userIter++)
    {
        u = *userIter;
        rui = uiMat[u][i];
        ruj = uiMat[u][j];

        if ( rui == 0.0 || ruj == 0.0 )
            continue;

        ave = uiMat[u][maxMovie];
        numerator += (rui - ave) * (ruj -ave);
        root1 += pow(rui - ave, 2);
        root2 += pow(ruj - ave, 2);
    }

    float root = sqrt(root1) * sqrt(root2);
    if ( root == 0.0 )
        return 0.0;

    return numerator / root;
}

float predictRating(int user, int movie, priority_queue<ItemSimilarity>* _neighborMovieList)
{
    int count = 0;
    int neighborMovie;
    float numerator = 0.0, root = 0.0, sim;
    float ramda = 0.7;
    float rate;


    priority_queue<ItemSimilarity>& neighborMovieList(_neighborMovieList[movie]);

    if ( neighborMovieList.size() == 0 )
        rate = ramda * uiMat[user][maxMovie] + (1-ramda)*uiMat[maxUser][movie];
    else
    {
        while( count < K && neighborMovieList.size() > 0 )
        {
            const ItemSimilarity& neighbor( neighborMovieList.top() );
            neighborMovie = neighbor.movieid_;
            sim = neighbor.sim_;
            numerator += sim * uiMat[user][movie];
            root += sim;
            count++;
            neighborMovieList.pop();
        }

        if ( root == 0.0 )
            return 0.0;

        rate = numerator / root;
    }

    if (rate > 5.0)
        return 5.0;
    else if ( rate < 1.0 )
        return 1.0;
    return rate;
}
