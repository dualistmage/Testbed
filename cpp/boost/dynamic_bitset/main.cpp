///
/// @brief test code of boost::dynamic_bitset
/// @author Dohyun Yun ( dualistmage@gmail.com )
/// @date 2010.12.09
///

#include <iostream>
#include <vector>
#include <boost/dynamic_bitset.hpp>

using namespace std;

int main(int argc, char* argv[])
{
    { // set dynamic_bitset of small and large number

        const size_t LARGE_NUM = 999999999;
        boost::dynamic_bitset<> bit(LARGE_NUM+1, 0);

        bit.set(3); // Set 3rd bit to 1
        bit.set(LARGE_NUM);

        cout << "bit 0, 3, " << LARGE_NUM << " : " << bit[0] << " , " << bit[3] << " , " << bit[LARGE_NUM] << endl;
        cout << "bit count : " << bit.count() << endl;
        
    } 

    { // set dynamic_bitset without any initialization
        boost::dynamic_bitset<> bit;

        const size_t LARGE_NUM = 999;
        bit.set(3); // Set 3rd bit to 1
        //bit.set(LARGE_NUM);
        cout << "bit 0, 3, " << LARGE_NUM << " : " << bit[0] << " , " << bit[3] << endl;
        //cout << "bit 0, 3, " << LARGE_NUM << " : " << bit[0] << " , " << bit[3] << " , " << bit[LARGE_NUM] << endl;
        cout << "bit count : " << bit.count() << endl;
    }
}
