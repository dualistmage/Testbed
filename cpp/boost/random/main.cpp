///
/// @brief test code of boost::random
/// @author Dohyun Yun ( dualistmage@gmail.com )
/// @date 2010.06.21
///

#include <iostream>
#include <boost/random.hpp>

using namespace std;

template<typename RealType>
inline RealType gen_random_real(RealType min, RealType max)
{
    static boost::mt19937 rng;
    typedef boost::uniform_real<RealType> random_range_t;
    random_range_t u(min, max);
    boost::variate_generator<boost::mt19937&, random_range_t> generator(rng, u);
    return generator();
}

int main(int argc, char* argv[])
{
    { // Test1
        cout << gen_random_real<double>(-10000.0f , 10000.0f) << endl;
        cout << gen_random_real<double>(-10.0f , 10.0f) << endl;
        cout << gen_random_real<double>(-100000.0f , 100000.0f) << endl;
    }
}
