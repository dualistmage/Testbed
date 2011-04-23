#include <iostream>
#include <fstream>
#include <math.h>
#include <list>

using namespace std;

const int maxUser = 944;
const int maxItem = 1683;
const bool PEARSON = false;
const bool COSINE = true;

float abs(float num)
{
    if ( num < 0.0 )
        return -num;
    return num;
}

// global functions
float getPearsonSimilarity(bool isUserBased, int index1, int index2, float** uiMat)
{
    int total = 0;
    list<int> corrIndexList;
    int r1, r2;
    float ave1 = 0.0, ave2 = 0.0;
    float root1 = 0.0, root2 = 0.0, numerator = 0.0;
    float diff1, diff2;
    if ( isUserBased )
    {
        // find correlated index and calculate each average.
        for(int i = 1; i < maxItem; i++)
        {
            r1 = uiMat[index1][i];
            r2 = uiMat[index2][i];
            if( r1 > 0 && r2 > 0 )
            {
                total++;
                corrIndexList.push_back(i);
                ave1 += r1;
                ave2 += r2;
            }
        }
        ave1 /= static_cast<float>(total);
        ave2 /= static_cast<float>(total);

        for(list<int>::iterator iter = corrIndexList.begin(); iter != corrIndexList.end(); iter++)
        {
            diff1 = uiMat[index1][*iter] - ave1;
            diff2 = uiMat[index2][*iter] - ave2;
            root1 += pow( diff1 , 2 );
            root2 += pow( diff2 , 2 );
            numerator += diff1 * diff2;
        }

    } // end - if ( isUserBased )
    else
    {
        // find correlated index and calculate each average.
        for(int i = 1; i < maxUser; i++)
        {
            r1 = uiMat[i][index1];
            r2 = uiMat[i][index2];
            if( r1 > 0 && r2 > 0 )
            {
                total++;
                corrIndexList.push_back(i);
                ave1 += r1;
                ave2 += r2;
            }
        }
        ave1 /= static_cast<float>(total);
        ave2 /= static_cast<float>(total);

        for(list<int>::iterator iter = corrIndexList.begin(); iter != corrIndexList.end(); iter++)
        {
            diff1 = uiMat[*iter][index1] - ave1;
            diff2 = uiMat[*iter][index2] - ave2;
            root1 += pow( diff1 , 2 );
            root2 += pow( diff2 , 2 );
            numerator += diff1 * diff2;
        }
    }

    float root = sqrt(root1) * sqrt(root2);
    if ( root == 0.0 )
        return 0.0;
    else
        return numerator / root;
} // end - float getPearsonSimilarity()

float getCosineSimilarity(bool isUserBased, int index1, int index2, float** uiMat)
{
    float r1, r2;
    float root1=0.0, root2=0.0, numerator=0.0;

    if ( isUserBased )
    {
        for(int i = 1; i < maxItem; i++)
        {
            r1 = uiMat[index1][i];
            r2 = uiMat[index2][i];
            //if( r1 > 0 && r2 > 0 )
            {
                root1 += r1*r1;
                root2 += r2*r2;
                numerator += r1*r2;
            }
        }
    } // end - if ( isUserBased )
    else
    {
        for(int i = 1; i < maxUser; i++)
        {
            r1 = uiMat[i][index1];
            r2 = uiMat[i][index2];
            //if( r1 > 0 && r2 > 0 )
            {
                root1 += r1*r1;
                root2 += r2*r2;
                numerator += r1*r2;
            }
        }
    }

    float root = sqrt(root1) * sqrt(root2);

    if ( root == 0.0 )
        return 0.0;
    else
        return numerator / root;
} // end - float getCosineSimilarity()

float getUserBasedRating(bool isCosine, int userid, int movieid, float** uiMat)
{
    float rui, w, numerator = 0.0, root = 0.0;
    for(int i = 1; i < maxUser; i++)
    {
        rui = uiMat[i][movieid];
        if ( rui == 0.0 ) 
            continue;

        if ( isCosine )
            w = getCosineSimilarity(true, userid, i, uiMat);
        else
            w = getPearsonSimilarity(true, userid, i, uiMat);

        numerator += (rui - uiMat[i][maxItem]) * w;
        root += abs(w);
    }
    if ( root == 0.0 )
        return 0;
    return ( uiMat[userid][maxItem] + numerator / root );
}
float getItemBasedRating(bool isCosine, int userid, int movieid, float** uiMat)
{
    float run, w, numerator = 0.0, root = 0.0;
    for(int i = 1; i < maxItem; i++)
    {
        run = uiMat[userid][i];
        if ( run == 0.0 )
            continue;

        if ( isCosine )
            w = getCosineSimilarity(false, movieid, i, uiMat);
        else
            w = getPearsonSimilarity(false, movieid, i, uiMat);

        numerator += run*w;
        root += abs(w);
    }
    if ( root == 0.0 )
        return 0;
    return numerator / root;
}

int main(int argc, char* argv[])
{
    if ( argc < 3 )
    {
        cout << "[Usage] cftest <basefile> <testfile>" << endl;
        return -1;
    }

    // read basefile and fill uiMat
    float** uiMat; // [userid][movieid]
    int* count;

    uiMat = new float*[maxUser];
    count = new int[maxUser];
    for(int i = 1; i < maxUser; i++)
    {
        uiMat[i] = new float[maxItem + 1]; // [maxItem] is average room for each user.
        for(int j = 1; j < maxItem+1; j++)
            uiMat[i][j] = 0.0;

        count[i] = 0;
    }


    ifstream basefile(argv[1]), testfile(argv[2]);

    if ( !basefile.is_open() )
    {
        cout << "basefile (" << argv[1] << ") is not opened" << endl;
        return -2;
    }
    if ( !testfile.is_open() )
    {
        cout << "testfile (" << argv[2] << ") is not opened" << endl;
        return -2;
    }

    int userid, movieid, rating, timestamp;

    // init matrix
    while( basefile.good() )
    {
        basefile >> userid >> movieid >> rating >> timestamp;
        if ( !basefile.good() )
            break;
        uiMat[userid][movieid] = rating;
        if( rating > 0.0 ) 
            count[userid]++;
        uiMat[userid][maxItem] += rating;
    }
    basefile.close();

    // calculate average score for each user
    for(int i = 1; i < maxUser; i++)
        uiMat[i][maxItem] /= static_cast<float>(count[i]);

    float userPearsonRating, userCosineRating, itemPearsonRating, itemCosineRating;
    float userPearsonScore, userCosineScore, itemPearsonScore, itemCosineScore;
        
    userPearsonScore = userCosineScore = itemPearsonScore =  itemCosineScore = 0.0;
    int testCount = 0;

    while( testfile.good() )
    {
        testfile >> userid >> movieid >> rating >> timestamp;
        if ( !testfile.good() )
            break;

        userPearsonRating = getUserBasedRating (PEARSON, userid, movieid, uiMat);
        userCosineRating  = getUserBasedRating (COSINE,  userid, movieid, uiMat);
        itemPearsonRating = getItemBasedRating (PEARSON, userid, movieid, uiMat);
        itemCosineRating  = getItemBasedRating (COSINE,  userid, movieid, uiMat);

        userPearsonScore += pow( userPearsonRating - rating , 2 );
        userCosineScore  += pow( userCosineRating  - rating , 2 );
        itemPearsonScore += pow( itemPearsonRating - rating , 2 );
        itemCosineScore  += pow( itemCosineRating  - rating , 2 );

        testCount++;
    }

    userPearsonScore /= static_cast<float>(testCount);
    userCosineScore  /= static_cast<float>(testCount);
    itemPearsonScore /= static_cast<float>(testCount);
    itemCosineScore  /= static_cast<float>(testCount);

    cout << "Result" << endl;
    cout << "==============================================" << endl;
    cout << "User-Cosine  Score : " << sqrt(userCosineScore)  << endl;
    cout << "User-Pearson Score : " << sqrt(userPearsonScore) << endl;
    cout << "Item-Cosine  Score : " << sqrt(itemCosineScore)  << endl;
    cout << "Item-Pearson Score : " << sqrt(itemPearsonScore) << endl;

    // deallocate memory
    delete[] count;
    for(int i = 1; i < maxUser; i++)
        delete[] uiMat[i];
    delete[] uiMat;
} 
