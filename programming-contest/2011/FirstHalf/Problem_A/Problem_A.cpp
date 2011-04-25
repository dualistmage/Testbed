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
    RatingUnit(int uid, int mid, double r) : userid_(uid), movieid_(mid), rating_(r) {};
    int userid_;
    int movieid_;
    double rating_;
}; // class RatingUnit

class ItemSimilarity
{
public:
    ItemSimilarity():id_(0), sim_(0.0f){};
    ItemSimilarity(int mid, double sim) : id_(mid), sim_(sim) {};
    bool operator< (const ItemSimilarity& rhs) const { 
        if ( fabs(this->sim_-rhs.sim_) < 0.0000001f )
            return this->id_ > rhs.id_;
        return this->sim_ < rhs.sim_; 
    }
    int id_;
    double sim_;
}; // class RatingUnit

// ------------------------------------------------------------------ [ Global variables ]
int K = 30;
int maxUser, maxMovie;
double** uiMat; // [userid][movieid]
double* userCount;
double* movieCount;
set<int> userList;
set<int> movieList;

ItemSimilarity** neighborList; // [movieid] // top K 

// ------------------------------------------------------------------ [ Function Definition ]
double getAdjustSimilarity(int movie1, int movie2); // movie1 must be smaller than movie2
double predictRating(int user, int movie, ItemSimilarity* simMovieList);
int itemCompare (const void* _lhs, const void* _rhs)
{
    ItemSimilarity* lhs = (ItemSimilarity*)_lhs;
    ItemSimilarity* rhs = (ItemSimilarity*)_rhs;

    if ( fabs(lhs->sim_-rhs->sim_) < 0.0000001f ) //equal
    {
        if ( lhs->id_ < rhs->id_ )
            return -1;
        else if ( lhs->id_ > rhs->id_ )
            return 1;
        else
            return 0;
    }
    else if ( lhs->sim_ < rhs->sim_ )
        return 1;
    return -1;
}


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
    //ofstream itemOut("item.out");


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
    uiMat = new double*[maxUser + 1]; // uiMat[maxUser][maxItem] is average room for each user or item.
    userCount = new double[maxUser + 1]; // count is for calculating average
    movieCount = new double[maxMovie + 1]; // count is for calculating average

    for(userIter = userList.begin(); userIter != userList.end(); userIter++)
    {
        i = *userIter;
        uiMat[i] = new double[maxMovie + 1];
        for(movieIter = movieList.begin(); movieIter != movieList.end(); movieIter++)
            uiMat[i][*movieIter] = 0.0;
        userCount[i] = 0.0;
    }

    uiMat[maxUser] = new double[maxMovie]; // for Movie Average
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
        uiMat[*userIter][maxMovie] /= static_cast<double>(userCount[*userIter]);
    for(movieIter = movieList.begin(); movieIter != movieList.end(); movieIter++)
        uiMat[maxUser][*movieIter] /= static_cast<double>(movieCount[*movieIter]);

    // Calculating similarity for each movie. The matrix is symmetry.
    //cout << "Start calculating similarity" << endl; // DEBUG

    ///////////////////////////////////////////////
    neighborList = new ItemSimilarity*[maxMovie];
    for(movieIter = movieList.begin(); movieIter != movieList.end(); movieIter++)
    {
        neighborList[*movieIter] = new ItemSimilarity[maxMovie];
        neighborList[*movieIter][0].id_ = 0; // count
    }

    int index;
    for(movieIter = movieList.begin(); movieIter != movieList.end(); movieIter++)
    {
        for(movieIter2 = movieList.begin(); movieIter2 != movieList.end(); movieIter2++)
        {
            i = *movieIter; j = *movieIter2;
            if (i >= j)
                continue;

            double sim = getAdjustSimilarity(i,j);

            if ( sim <= 0.0f )
                continue;

            index = ++(neighborList[i][0].id_);
            neighborList[i][index].id_ = j;
            neighborList[i][index].sim_ = sim;

            index = ++(neighborList[j][0].id_);
            neighborList[j][index].id_ = i;
            neighborList[j][index].sim_ = sim;

//             neighborList[i].push( ItemSimilarity(j,sim) );
//             neighborList[j].push( ItemSimilarity(i,sim) );

        }
    }

    // sort and cut
    for(movieIter = movieList.begin(); movieIter != movieList.end(); movieIter++)
    {
        qsort( &neighborList[*movieIter][1], neighborList[*movieIter][0].id_, sizeof(ItemSimilarity), itemCompare);
        neighborList[*movieIter][0].id_ = min( neighborList[*movieIter][0].id_ , K );
    }

//     //DEBUG
//     for(movieIter = movieList.begin(); movieIter != movieList.end(); movieIter++)
//     {
//         i = *movieIter;
//         ItemSimilarity* neighborMovieList(neighborList[i]);
//         int count = neighborMovieList[0].id_;
//         itemOut << "[\t" << i << "]\t" << movieCount[i] << ":\t" << uiMat[maxUser][i] << "[\t" << count << "\t->";
//         int index = 1;
// 
//         while( index <= count )
//         {
//             itemOut << "\t" << neighborMovieList[index].id_ << "(" << neighborMovieList[index].sim_ << ")";
//             index++;
//         }
//         itemOut << endl;
//     }
//     itemOut.close();



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
//             cerr << "\t" << neighborMovieList.top().id_;
//             count++;
//             neighborMovieList.pop();
//         }
//         cerr << endl;
//     }
//     // DEBUG


    /*************

    // DEBUG - start
    i = 1;
    cout << i;
    j = 24;     cout << "\t" << j << "(" << predictRating(i,j, neighborList) << ")"<< endl;;
    j = 1111;   cout << "\t" << j << "(" << predictRating(i,j, neighborList) << ")"<< endl;;
    j = 1127;   cout << "\t" << j << "(" << predictRating(i,j, neighborList) << ")"<< endl;;
    j = 268;    cout << "\t" << j << "(" << predictRating(i,j, neighborList) << ")"<< endl;;
    j = 318;    cout << "\t" << j << "(" << predictRating(i,j, neighborList) << ")"<< endl;;
    j = 501;    cout << "\t" << j << "(" << predictRating(i,j, neighborList) << ")"<< endl;;
    j = 1511;   cout << "\t" << j << "(" << predictRating(i,j, neighborList) << ")"<< endl;;
    j = 1191;   cout << "\t" << j << "(" << predictRating(i,j, neighborList) << ")"<< endl;;
    j = 1347;   cout << "\t" << j << "(" << predictRating(i,j, neighborList) << ")"<< endl;;
    j = 1357;   cout << "\t" << j << "(" << predictRating(i,j, neighborList) << ")"<< endl;;
    cout << endl;

    ***********************/

    // DEBUG - end

    // Calculating top k neighbors for each item.
    double pui;
    size_t topk = 10;

    ItemSimilarity* recommendedMovieList = new ItemSimilarity[maxMovie];
    for(userIter = userList.begin(); userIter != userList.end(); userIter++)
    {
        recommendedMovieList[0].id_ = 0;
        //cout << "Calculate user : " << *userIter << endl; // DEBUG
        i = *userIter;
        for(movieIter = movieList.begin(); movieIter != movieList.end(); movieIter++)
        {
            j = *movieIter;
            if ( uiMat[i][j] != 0.0 )
                continue;
            pui = predictRating( i, j, neighborList[j] );
            recommendedMovieList[ recommendedMovieList[0].id_ ].id_ = j;
            recommendedMovieList[ recommendedMovieList[0].id_ ].sim_ = pui;
            recommendedMovieList[0].id_++;
        }

        qsort( &recommendedMovieList[1], recommendedMovieList[0].id_, sizeof(ItemSimilarity), itemCompare);

        // Output
        fpout << i;
        size_t rMovieSize = recommendedMovieList[0].id_;
        for(size_t i = 1; i <= rMovieSize && i < topk; i++)
        {
            fpout << "\t" << recommendedMovieList[i].id_;// << "(" << recommendedMovieList[i].sim_ << ")";
        }
        fpout << endl;
    }

    fpin.close();
    fpout.close();

    //*/

} // end - int main()




// ------------------------------------------------------------------ [ Function Implementation ]
double getAdjustSimilarity(int i, int j)
{
    double ave;
    double numerator = 0.0, root1 = 0.0, root2 = 0.0;
    double rui, ruj;
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

    double root = sqrt(root1) * sqrt(root2);
    if ( root == 0.0 )
        return 0.0;

    return numerator / root;
}

double predictRating(int user, int movie, ItemSimilarity* neighborMovieList)
{
    int count = 0;
    int neighborMovie;
    double numerator = 0.0, root = 0.0, sim;
    double ramda = 0.7;
    double rate;

    int size = neighborMovieList[0].id_;

    if ( size == 0 )
    {
        rate = ramda * uiMat[user][maxMovie] + (1-ramda)*uiMat[maxUser][movie];
    }
    else
    {
        int index = 1;
        while( count < K && index <= size )
        {
            const ItemSimilarity& neighbor( neighborMovieList[index] );
            neighborMovie = neighbor.id_;
            sim = neighbor.sim_;
            numerator += sim * uiMat[user][neighborMovie];
            //cerr << "Neighbor similarity (" << user << "-" << movie << ") : " << count << " = " << neighborMovie << "-" << sim << "#" << uiMat[user][neighborMovie] << endl;
            root += sim;
            count++;
            index++;
        }

        if ( root == 0.0 )
            return 0.0;

        rate = numerator / root;
    }

    /*
    if (rate > 5.0)
        return 5.0;
    else if ( rate < 1.0 )
        return 1.0;
        */
    return rate;
}

