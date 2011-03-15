#include <iostream>
#include <math.h>

#include <boost/timer.hpp>
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

inline float fast_log1(float val) 
{                                
    float result, tmp;           
    float mp = 0.346607f;        

    result = *(int*)&val;        
    result *= 1.0/(1<<23);       
    result = result - 127;       

    tmp = result - floor(result);
    tmp = (tmp - tmp*tmp) * mp;  
    return tmp + result;         
} // end - fast_log()            

inline float fast_log2 (float val)
{
    int * const  exp_ptr = reinterpret_cast <int *> (&val);
    int          x = *exp_ptr;
    const int    log_2 = ((x >> 23) & 255) - 128;
    x &= ~(255 << 23);
    x += 127 << 23;
    *exp_ptr = x;

    return ((val + log_2) * 0.69314718f);
} 



int main(int argc, char* argv[])
{
    size_t dataSize = 100;
    float dataMin = 1, dataMax = 999;
    vector<float> dataSet(dataSize), answerSet(dataSize);
    for(size_t i = 0; i < dataSize; i++)
    {
        dataSet[i] = gen_random_real<float>(dataMin, dataMax);
        cout << "Random[" << i << "] : " << dataSet[i] << endl;
    }

    boost::timer timer;
    std::vector<double> elapseList, errorList;
    double errorSum, localLog;

    // Normal log function
    timer.restart();
    for(size_t i = 0; i < dataSize; i++)
    {
        localLog = log( dataSet[i] );
        //if ( i == 0 ) 
            cout << "Log : " << localLog << endl;
        answerSet[i] = localLog;

        // check error rate;
        errorSum += abs(localLog - answerSet[i]);
    }
    elapseList.push_back( timer.elapsed() );
    errorList.push_back(errorSum / dataSize);

    // log function 1
    timer.restart();
    for(size_t i = 0; i < dataSize; i++)
    {
        localLog = fast_log1( dataSet[i] );
        //if ( i == 0 ) 
            cout << "Log1 : " << localLog << endl;

        // check error rate;
        errorSum += abs(localLog - answerSet[i]);
    }
    elapseList.push_back( timer.elapsed() );
    errorList.push_back(errorSum / dataSize);

    // log function 2
    timer.restart();
    for(size_t i = 0; i < dataSize; i++)
    {
        localLog = fast_log2( dataSet[i] );
        //if ( i == 0 ) 
            cout << "Log2 : " << localLog << endl;

        // check error rate;
        errorSum += abs(localLog - answerSet[i]);
    }
    elapseList.push_back( timer.elapsed() );
    errorList.push_back(errorSum / dataSize);


    for(size_t i = 0; i < elapseList.size(); i++)
        cout << "Log type [" << i << "] : " << elapseList[i] << " - Error : " << errorList[i] << endl;


} // end - main()
