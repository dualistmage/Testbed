#include <iostream>
#include <stdio.h>
#include <math.h>

using namespace std;

///
/// round function
/// ex) num= 10.2465, where=3 ->  10.25
/// ex) num=-10.2465, where=3 -> -10.25
///
double round(double num, int where)
{
    double roundedNum = num * pow(10, where-1);
    roundedNum += 0.5;
    roundedNum = (roundedNum>0)?floor(roundedNum):ceil(roundedNum);
    roundedNum *= pow(10, -1 * (where-1));
    return roundedNum;
}

int main()
{
    double a,b,c,p,q,r;
    double dist;
    double first, second, third;

    int testNo;
    cin >> testNo;

    while(testNo)
    {
        cin >> a >> b >> c >> p >> q >> r;
        if ( a == 0.0 )
        {
            first = q + c/b;
            if (first < 0.0)
                first *= -1;
            dist = first - r;
        }
        else if ( b == 0.0 )
        {
            first = q + c/a;
            if (first < 0.0)
                first *= -1;
            dist = first - r;
        }
        else
        {
            first = pow(a,2) / pow(b,2);
            second = (a*p - b*q - c) / 2 * a;
            third = p + r / ( sqrt(1 + pow(a,2)/pow(b,2)) );
            dist = sqrt( first * (second - third) );
        }
        if ( dist < 0.0 )
            dist = 0;

        printf("%.4lf\n", dist);
        testNo--;
    }


}
