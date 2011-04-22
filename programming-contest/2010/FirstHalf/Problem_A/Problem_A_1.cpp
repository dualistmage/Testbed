///
/// @file   Problem_A_1.cpp
/// @author Dohyun Yun
/// @date   2010.01.09
///

#include <iostream>
#include <fstream>

#include "Profile.h"

using namespace std;

int main(int argc, char* argv[] )
{
    if ( argc != 2 )
    {
        cout << "[Usage] Problem_A_1 <profile>" << endl;
        return 0;
    } // end - if

    // ---------------------------------------- [ Read Profile
    ifstream fpin( argv[1] );
    if ( !fpin.is_open() )
    {
        cerr << "[Error] File (" << argv[1] << ") is not opened." << endl;
        return -1;
    }

    while( fpin.good() )
    {
        std::string trainPath, profilePath, categoryName;
        fpin >> trainPath >> profilePath >> categoryName;

        if ( !fpin.good() )
            break;

        Profile profile(categoryName);
        profile_tool::makeProfile(trainPath, profile);
        profile.fileOut(profilePath);
    } // end - while

} // end - main()

