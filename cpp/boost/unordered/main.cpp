///
/// @brief test code of boost::unordered_map
/// @author Dohyun Yun ( dualistmage@gmail.com )
/// @date 2010.06.21
///

#include <iostream>
#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>

using namespace std;

int main(int argc, char* argv[])
{
    { // Exception Test : use unexpected value in the operator[]
        boost::unordered_map<char,bool> test;
        boost::unordered_map<char,int> test2;
        test.insert( make_pair( '[' , true ) );
        test2.insert( make_pair( '[' , 333 ) );

        cout << "<char,bool> value - Expected Key : " << test['['] << " , Unexpected Key : " << test['?'] << endl;
        cout << "<char,int> value - Expected Key : " << test2['['] << " , Unexpected Key : " << test2['?'] << endl;
    } 
    { // Check if unordered_set doesn't mean it maintains its order.
        typedef boost::unordered_set<std::string> uset;
        uset test;
        test.insert( "Hello" );
        test.insert( "Africa" );
        test.insert( "Center" );
        test.insert( "Park" );
        test.insert( "Brather" );

        for(uset::iterator iter = test.begin(); iter != test.end(); iter++)
            cout << "Term : " << *iter << endl;
    }
}
