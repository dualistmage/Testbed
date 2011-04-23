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

deque<ItemSimilarity>* itemNeighborList; // [movieid] // top K 
deque<ItemSimilarity>* userNeighborList; // [movieid] // top K 

// ------------------------------------------------------------------ [ Function Definition ]
double getItemAdjustSimilarity(int movie1, int movie2, int& count); // movie1 must be smaller than movie2
double getUserAdjustSimilarity(int user1, int user2, int& count); // movie1 must be smaller than movie2
double getItemSimilarity(int movie1, int movie2); // movie1 must be smaller than movie2
double getUserSimilarity(int user1, int user2); // movie1 must be smaller than movie2
double predictRating(
        int user, 
        int movie, 
        deque<ItemSimilarity>& simMovieList,
        deque<ItemSimilarity>& simUserList);


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
    cout << "Start calculating similarity" << endl; // DEBUG

    itemNeighborList = new deque<ItemSimilarity>[maxMovie];
    for(movieIter = movieList.begin(); movieIter != movieList.end(); movieIter++)
    {
        //movieIter2 = movieIter; movieIter2++;
        for(movieIter2 = movieList.begin(); movieIter2 != movieList.end(); movieIter2++)
        {
            i = *movieIter; j = *movieIter2;
            if (i <= j)
                continue;

            double sim = getItemSimilarity(i,j);

            if ( sim > THETA )
            {
                itemNeighborList[i].push_back( ItemSimilarity(j,sim) );
                itemNeighborList[j].push_back( ItemSimilarity(i,sim) );
            }

        }
    }

    // Output item neighbor list for each item.
    for(movieIter = movieList.begin(); movieIter != movieList.end(); movieIter++)
    {
        i = *movieIter;
        itemout << i;
        //movieIter2 = movieIter; movieIter2++;
        for(movieIter2 = movieList.begin(); movieIter2 != movieList.end(); movieIter2++)
        {
            j = *movieIter2;
            if ( i == j )
                continue;
            itemout << "\t" << j;
        }
        itemout << endl;
    }
    itemout.close();


    userNeighborList = new deque<ItemSimilarity>[maxUser];
    for(userIter = userList.begin(); userIter != userList.end(); userIter++)
    {
        for(userIter2 = userList.begin(); userIter2 != userList.end(); userIter2++)
        {
            i = *userIter; j = *userIter2;
            if (i <= j)
                continue;

            double sim = getUserSimilarity(i,j);

            if ( sim > THETA )
            {
                userNeighborList[i].push_back( ItemSimilarity(j,sim) );
                userNeighborList[j].push_back( ItemSimilarity(i,sim) );
            }
        }
    }

    // Output item neighbor list for each item.
    for(userIter = userList.begin(); userIter != userList.end(); userIter++)
    {
        i = *userIter;
        userout << i;
        //userIter2 = userIter; userIter2++;
        for(userIter2 = userList.begin(); userIter2 != userList.end(); userIter2++)
        {
            j = *userIter2;
            if ( i == j )
                continue;
            userout << "\t" << j;
        }
        userout << endl;
    }
    userout.close();


    // Calculating top k neighbors for each item.
    double pui;
    size_t topk = 10;

    for(userIter = userList.begin(); userIter != userList.end(); userIter++)
    {
        cout << "Calculate user : " << *userIter << endl; // DEBUG
        i = *userIter;
        priority_queue<ItemSimilarity> recommendedMovieList;
        for(movieIter = movieList.begin(); movieIter != movieList.end(); movieIter++)
        {
            j = *movieIter;
            if ( uiMat[i][j] != 0.0 )
                continue;
            pui = predictRating( i, j, itemNeighborList[i], userNeighborList[j] );
            recommendedMovieList.push( ItemSimilarity( j, pui ) );
        }

        // Output
        fpout << i;
        size_t rMovieSize = recommendedMovieList.size();
        for(size_t i = 0; i < rMovieSize && i < topk; i++)
        {
            fpout << "\t" << recommendedMovieList.top().id_ ;
            recommendedMovieList.pop();
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
        deque<ItemSimilarity>& simMovieList,
        deque<ItemSimilarity>& simUserList)
{
    int ua;
    int ik;
    double rate, sim;
    size_t mSize = simMovieList.size();
    size_t uSize = simUserList.size();
    double userRoot = 0.0f, userNumerator = 0.0f;
    double itemRoot = 0.0f, itemNumerator = 0.0f;


    deque<ItemSimilarity>::iterator userIter, itemIter;

    for(userIter = simUserList.begin(); userIter != simUserList.end(); userIter++)
    {
        ua = userIter->id_;
        sim = userIter->sim_;
        userNumerator += sim * (uiMat[ua][movie] - uiMat[ua][maxMovie]);
        userRoot += sim;
    }

    for(itemIter = simMovieList.begin(); itemIter != simMovieList.end(); itemIter++)
    {
        ik = itemIter->id_;
        sim = itemIter->sim_;
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
