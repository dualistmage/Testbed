///
/// @brief  Problem_A.cpp
/// @author Dohyun Yun ( dualistmage@wisenut.co.kr )
/// @date   2011.04.23
///

#include <iostream>

using namespace std;

// ------------------------------------------------------------------ [ Global variables ]
int maxUser, maxMovie;
float** uiMat; // [userid][movieid]
float* userCount;
float* movieCount;

// ------------------------------------------------------------------ [ Function Definition ]
float getAdjustSimilarity(int item1, int item2);

// ------------------------------------------------------------------ [ Class Definition ]
class RatingUnit
{
public:
    RatingUnit(int uid, mid, r) : userId_(uid), movieid(mid), rating_(r) {};
    int userid_;
    int movieid_;
    int rating_;
}; // class RatingUnit

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
    if ( !fpin.is_open() )
    {
        cout << "input file(" << argv[1] << ") is not opened" << endl;
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

        maxUser  = max(maxUser,userid);
        maxMovie = max(maxMovie,movieid);
        ratingList.push_back( RatingUnit(userid, movieid, rating) );

    } // end - while()

    maxUser++;
    maxMovie++;

    // Allocate ui matrix
    uiMat = new float*[maxUser + 1]; // uiMat[maxUser][maxItem] is average room for each user or item.
    userCount = new float[maxUser + 1]; // count is for calculating average
//     movieCount = new float[maxMovie + 1]; 

    for(i = 1; i < maxUser; i++)
    {
        uiMat[i] = new float[MaxMovie + 1];
        for(j = 1; j < maxMovie; j++)
            uiMat[i][j] = 0.0;

        userCount[i] = 0;
    }

//     for(i = 1; i < maxMovie; i++)
//     {
//         movieCount[i] = 0;
//     }
    
    // Assign values to uiMat, calculate average of user and movie
    list<RatingUnit>::iterator iter = ratingList.begin();
    for(; iter != ratingList.end(); iter++)
    {
        userid = iter->userid_;
        movieid = iter->movieid_;
        rating = iter->rating_;
        uiMat[userid][movieid] = rating;

        uiMat[userid][maxMovie] += rating;
        userCount[userid]++;

//         uiMat[maxUser][movieid] += rating;
//         movieCount[movieid]++;
    }

    for(i = 1; i < maxUser; i++)
        uiMat[i][maxMovie] \= static_cast<float>(userCount[i]);

//     for(i = 1; i < maxMovie; i++)
//         uiMat[maxUser][i] \= static_cast<float>(movieCount[i]);
    

    // Calculate the prediction for each user;


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
