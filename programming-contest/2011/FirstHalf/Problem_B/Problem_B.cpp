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
int maxUser, maxMovie;
double** uiMat; // [userid][movieid]
double* userCount;
double* movieCount;
set<int> userList;
set<int> movieList;

const int GAMMA     = 25;
const int DELTA     = 30;
const double ETA    = 0.3f;
const double THETA  = 0.3f;
const double LAMBDA = 0.7f;

ItemSimilarity** itemNeighborList; // [movieid] // top K 
ItemSimilarity** userNeighborList; // [movieid] // top K 

// ------------------------------------------------------------------ [ Function Definition ]
double getItemAdjustSimilarity(int movie1, int movie2, int& count); // movie1 must be smaller than movie2
double getUserAdjustSimilarity(int user1, int user2, int& count); // movie1 must be smaller than movie2
double getItemSimilarity(int movie1, int movie2); // movie1 must be smaller than movie2
double getUserSimilarity(int user1, int user2); // movie1 must be smaller than movie2
double predictRating(
        int user, 
        int movie, 
        ItemSimilarity* simMovieList,
        ItemSimilarity* simUserList);

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
    if ( argc < 5 )
    {
        cout << "[Usage] Problem_A <input> <user_neighbor_out> <item_neighbor_out> <top_n_recommended>" << endl;
        return -1;
    }

    ifstream fpin(argv[1]);
    ofstream userout(argv[2]);
    ofstream itemout(argv[3]);
    ofstream fpout(argv[4]);

    if ( !fpin.is_open() )
    {
        cout << "input file(" << argv[1] << ") is not opened" << endl;
        return -2;
    }
    if ( !userout.is_open() )
    {
        cout << "output file(" << argv[2] << ") is not opened" << endl;
        return -2;
    }
    if ( !itemout.is_open() )
    {
        cout << "output file(" << argv[3] << ") is not opened" << endl;
        return -2;
    }
    if ( !fpout.is_open() )
    {
        cout << "output file(" << argv[4] << ") is not opened" << endl;
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

    set<int>::iterator userIter, userIter2, movieIter, movieIter2;

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

    /////////////////////////////// Item
    itemNeighborList = new ItemSimilarity*[maxMovie];
    for(movieIter = movieList.begin(); movieIter != movieList.end(); movieIter++)
    {
        itemNeighborList[*movieIter] = new ItemSimilarity[maxMovie];
        itemNeighborList[*movieIter][0].id_ = 0; // count
    }
    for(movieIter = movieList.begin(); movieIter != movieList.end(); movieIter++)
    {
        int index = 0;
        //movieIter2 = movieIter; movieIter2++;
        for(movieIter2 = movieList.begin(); movieIter2 != movieList.end(); movieIter2++)
        {
            i = *movieIter; j = *movieIter2;
            if (i >= j)
                continue;

            double sim = getItemSimilarity(i,j);

            if ( sim > THETA )
            {
                //cout << "Item Similarity (" << i << "," << j << ") : " << sim << endl;
                index = ++(itemNeighborList[i][0].id_);
                itemNeighborList[i][index].id_ = j;
                itemNeighborList[i][index].sim_ = sim;
                index = ++(itemNeighborList[j][0].id_);
                itemNeighborList[j][index].id_ = i;
                itemNeighborList[j][index].sim_ = sim;
            }

        }
    }

    // sort and cut
    for(movieIter = movieList.begin(); movieIter != movieList.end(); movieIter++)
    {
        qsort( &itemNeighborList[*movieIter][1], itemNeighborList[*movieIter][0].id_, sizeof(ItemSimilarity), itemCompare);
    }

    // Output item neighbor list for each item.
    for(movieIter = movieList.begin(); movieIter != movieList.end(); movieIter++)
    {
        i = *movieIter;
        itemout << i;
        //movieIter2 = movieIter; movieIter2++;
        ItemSimilarity* neighborMovieList(itemNeighborList[i]);
        int count = neighborMovieList[0].id_;
        int index = 1;

        while( index <= count )
        {
            itemout << "\t" << neighborMovieList[index].id_;
            index++;
        }
        itemout << endl;
    }
    itemout.close();


    /////////////////////////////// user
    userNeighborList = new ItemSimilarity*[maxUser];
    for(userIter = userList.begin(); userIter != userList.end(); userIter++)
    {
        userNeighborList[*userIter] = new ItemSimilarity[maxUser];
        userNeighborList[*userIter][0].id_ = 0; // count
    }
    for(userIter = userList.begin(); userIter != userList.end(); userIter++)
    {
        int index = 0;
        //movieIter2 = movieIter; movieIter2++;
        for(userIter2= userList.begin(); userIter2 != userList.end(); userIter2++)
        {
            i = *userIter; j = *userIter2;
            if (i >= j)
                continue;

            double sim = getUserSimilarity(i,j);

            if ( sim > ETA )
            {
                //cout << "User Similarity (" << i << "," << j << ") : " << sim << endl;
                index = ++(userNeighborList[i][0].id_);
                userNeighborList[i][index].id_ = j;
                userNeighborList[i][index].sim_ = sim;
                index = ++(userNeighborList[j][0].id_);
                userNeighborList[j][index].id_ = i;
                userNeighborList[j][index].sim_ = sim;
            }

        }
    }

    // sort and cut
    for(userIter = userList.begin(); userIter != userList.end(); userIter++)
    {
        qsort( &userNeighborList[*userIter][1], userNeighborList[*userIter][0].id_, sizeof(ItemSimilarity), itemCompare);
    }

    // Output item neighbor list for each item.
    for(userIter = userList.begin(); userIter != userList.end(); userIter++)
    {
        i = *userIter;
        userout << i;
        //movieIter2 = movieIter; movieIter2++;
        ItemSimilarity* neighborUserList(userNeighborList[i]);
        int count = neighborUserList[0].id_;
        int index = 1;

        while( index <= count )
        {
            userout << "\t" << neighborUserList[index].id_;
            index++;
        }
        userout << endl;
    }
    userout.close();



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
            pui = predictRating( i, j, itemNeighborList[j], userNeighborList[i] );
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
            fpout << "\t" << recommendedMovieList[i].id_; // << "(" << recommendedMovieList[i].sim_ << ")";
        }
        fpout << endl;
    }

    fpin.close();
    fpout.close();

} // end - int main()




// ------------------------------------------------------------------ [ Function Implementation ]
double getItemAdjustSimilarity(int i, int j, int& count)
{
    count = 0;
    double avei, avej;
    double numerator = 0.0f, root1 = 0.0f, root2 = 0.0f;
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

        count++;
        avei = uiMat[maxUser][i];
        avej = uiMat[maxUser][j];
        numerator += (rui - avei) * (ruj -avej);
        root1 += pow(rui - avei, 2);
        root2 += pow(ruj - avej, 2);
    }

    double root = sqrt(root1) * sqrt(root2);
    if ( root == 0.0f )
        return 0.0f;

    return numerator / root;
}

double getUserAdjustSimilarity(int a, int u, int& count)
{
    count = 0;
    double avea, aveu;
    double numerator = 0.0f, root1 = 0.0f, root2 = 0.0f;
    double rai, rui;
    int i;
    set<int>::iterator movieIter = movieList.begin();
    for(; movieIter != movieList.end(); movieIter++)
    {
        i = *movieIter;
        rai = uiMat[a][i];
        rui = uiMat[u][i];

        if ( rai == 0.0 || rui == 0.0 )
            continue;

        count++;
        avea = uiMat[a][maxMovie];
        aveu = uiMat[u][maxMovie];
        numerator += (rai - avea) * (rui -aveu);
        root1 += pow(rai - avea, 2);
        root2 += pow(rui - aveu, 2);
    }

    double root = sqrt(root1) * sqrt(root2);
    if ( root == 0.0 )
        return 0.0;

    return numerator / root;
}

double getItemSimilarity(int movie1, int movie2)
{
    int count;
    double sim = getItemAdjustSimilarity(movie1, movie2, count);
    return ( min(count,DELTA) / DELTA * sim );
}

double getUserSimilarity(int user1, int user2)
{
    int count;
    double sim = getUserAdjustSimilarity(user1, user2, count);
    return min(count,GAMMA) / GAMMA * sim;
}

double predictRating(
        int user, 
        int movie, 
        ItemSimilarity* simMovieList,
        ItemSimilarity* simUserList)
{
    int ua;
    int ik;
    double rate, sim;
    //size_t mSize = (simMovieList[0] == 0x00)?0:simMovieList[0].id_;
    //size_t uSize = (simUserList[0] == 0x00)?0:simUserList[0].id_;
    size_t mSize = simMovieList[0].id_;
    size_t uSize = simUserList[0].id_;
    double userRoot = 0.0f, userNumerator = 0.0f;
    double itemRoot = 0.0f, itemNumerator = 0.0f;


    deque<ItemSimilarity>::iterator userIter, itemIter;

    for(size_t i = 1; i <= uSize; i++)
    {
        ua = simUserList[i].id_;
        sim = simUserList[i].sim_;
        userNumerator += sim * (uiMat[ua][movie] - uiMat[ua][maxMovie]);
        userRoot += sim;
    }

    for(size_t i = 1; i <= mSize; i++)
    {
        ik = simMovieList[i].id_;
        sim = simMovieList[i].sim_;
        itemNumerator += sim * (uiMat[user][ik] - uiMat[maxUser][ik]);
        itemRoot += sim;
    }

    if ( uSize != 0 && mSize != 0 )
        rate = LAMBDA * ( uiMat[user][maxMovie] + userNumerator / userRoot )
            + (1 - LAMBDA) * (uiMat[maxUser][movie] + itemNumerator / itemRoot );
    else if ( uSize != 0 && mSize == 0 )
        rate = uiMat[user][maxMovie] + userNumerator / userRoot;
    else if ( uSize == 0 && mSize != 0 )
        rate = uiMat[maxUser][movie] + itemNumerator / itemRoot;
    else
        rate = LAMBDA * uiMat[user][maxMovie] + LAMBDA * uiMat[maxUser][movie];

    return rate;
}
