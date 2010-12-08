///
/// @brief test code of boost::prior and boost::next
/// @author Dohyun Yun ( dualistmage@gmail.com )
/// @date 2010.12.08
///

#include <iostream>
#include <vector>
#include <boost/utility.hpp>

namespace pn {
    template<typename T>
        T next(T x) { return ++x; }
    template<typename T, class Distance>
        T next(T x, Distance n) { std::advance(x, n); return x; }
    template<typename T>
        T prior(T x) { return --x; }
    template<typename T, class Distance>
        T prior(T x, Distance n) { std::advance(x, -n); return x; }
}

int main(int argc, char* argv[])
{
    { // access elements in the vector by using prior and next
        using namespace std;
        std::vector<int> vec(10);
        for(int i=0; i< 10; i++)
            vec[i] = i+1;

        cout << "Index : ";
        for(int i = 0; i < 10; i++)
            printf("%-5d", i);
        cout << endl << "vec   : ";
        for(int i = 0; i < 10; i++)
            printf("%-5d", vec[i]);
        cout << endl << endl;

        cout << "boost::next    of vec::begin() : " << *boost::next (vec.begin())    << endl;
        cout << "pn   ::next    of vec::begin() : " << *pn::next    (vec.begin())    << endl;
        cout << "boost::next 3  of vec::begin() : " << *boost::next (vec.begin(), 3) << endl;
        cout << "pn   ::next 3  of vec::begin() : " << *pn::next    (vec.begin(), 3) << endl;
        cout << "boost::prior   of vec::end()   : " << *boost::prior(vec.end())      << endl;
        cout << "pn   ::prior   of vec::end()   : " << *pn::prior   (vec.end())      << endl;
        cout << "boost::prior 2 of vec::end()   : " << *boost::prior(vec.end(), 2)   << endl;
        cout << "pn   ::prior 2 of vec::end()   : " << *pn::prior   (vec.end(), 2)   << endl;
    } 
}
