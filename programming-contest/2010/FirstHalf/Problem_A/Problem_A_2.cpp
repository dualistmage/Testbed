///
/// @file   Problem_A_2.cpp
/// @author Dohyun Yun
/// @date   2010.01.09
///

#include <iostream>
#include <fstream>

#include "Profile.h"

using namespace std;

int main(int argc, char* argv[] )
{

    // TEST
    // Profile dp, cp;
    // cp.ngram_.insert( make_pair("TH" , 8 ) );
    // cp.ngram_.insert( make_pair("ER" , 7 ) );
    // cp.ngram_.insert( make_pair("ON" , 6 ) );
    // cp.ngram_.insert( make_pair("LE" , 5 ) );
    // cp.ngram_.insert( make_pair("ING" , 4 ) );
    // cp.ngram_.insert( make_pair("AND" , 3 ) );
    // dp.ngram_.insert( make_pair("TH" , 8 ) );
    // dp.ngram_.insert( make_pair("ING" , 7 ) );
    // dp.ngram_.insert( make_pair("ON" , 6 ) );
    // dp.ngram_.insert( make_pair("ER" , 5 ) );
    // dp.ngram_.insert( make_pair("AND" , 4 ) );
    // dp.ngram_.insert( make_pair("ED" , 3 ) );
    // dp.makeProfileData(); cp.makeProfileData();
    // dp.buildIndexMap(); cp.buildIndexMap();
    // cout << "DISTANCE : " << dp.getDistance( cp ) << endl;
    // return 0;


    if ( argc != 4 )
    {
        cout << "[Usage] Problem_A_2 <profile> <input> <output>" << endl;
        return 0;
    } // end - if

    // ---------------------------------------- [ Read Profile
    ifstream fpin( argv[1] );
    if ( !fpin.is_open() )
    {
        cerr << "[Error] File (" << argv[1] << ") is not opened." << endl;
        return -1;
    }

    std::vector<std::string> trainPathList, profilePathList, categoryNameList;
    while( fpin.good() )
    {
        std::string trainPath, profilePath, categoryName;
        fpin >> trainPath >> profilePath >> categoryName;

        if ( !fpin.good() )
            break;
        trainPathList.push_back( trainPath );
        profilePathList.push_back( profilePath );
        categoryNameList.push_back( categoryName );
    } // end - while
    fpin.close();

    size_t listSize = trainPathList.size();
    std::vector<Profile> categoryList(listSize);
    for(size_t i = 0; i < listSize; i++)
    {
        categoryList[i].profileName_ = categoryNameList[i];
        categoryList[i].profilePath_ = profilePathList[i];
        profile_tool::makeProfile(trainPathList[i], categoryList[i]);
    }

    // ---------------------------------------- [ Read Input
    fpin.open( argv[2] );
    if ( !fpin.is_open() )
    {
        cerr << "[Error] File (" << argv[2] << ") is not opened." << endl;
        return -1;
    }
    ofstream fpout( argv[3] );
    if ( !fpout.is_open() )
    {
        cerr << "[Error] File (" << argv[3] << ") is not opened." << endl;
        return -1;
    }

    std::vector<std::string> resultList;
    while( fpin.good() )
    {
        std::string line, refinedStr;
        getline(fpin, line);
        profile_tool::replaceSpace(line, refinedStr);
        Profile documentProfile("testing");
        documentProfile.makeNGram(refinedStr, 5);
        resultList.push_back( profile_tool::getCategoryName(documentProfile, categoryList) );
    } // end - while()

    for(std::vector<std::string>::iterator rIter = resultList.begin();
            rIter != resultList.end(); rIter++)
    {
        fpout << *rIter;
        if ( rIter + 1 != resultList.end() )
            fpout << endl;
    }

    fpin.close();
    fpout.close();

    return 0;

} // end - main()
