///
/// @brief test code of boost::unordered_map
/// @author Dohyun Yun ( dualistmage@gmail.com )
/// @date 2010.06.21
///

#include <iostream>
#include <boost/unordered_map.hpp>

int main(int argc, char* argv[])
{
    { // Exception Test : use unexpected value in the operator[]
        using namespace std;
        boost::unordered_map<char,bool> test;
        boost::unordered_map<char,int> test2;
        test.insert( make_pair( '[' , true ) );
        test2.insert( make_pair( '[' , 333 ) );

        cout << "<char,bool> value - Expected Key : " << test['['] << " , Unexpected Key : " << test['?'] << endl;
        cout << "<char,int> value - Expected Key : " << test2['['] << " , Unexpected Key : " << test2['?'] << endl;
    } 
}
