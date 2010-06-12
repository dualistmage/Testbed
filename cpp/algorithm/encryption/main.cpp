#include <iostream>
#include "a.h"

using namespace std;

int main(int argc, char* argv[])
{
    A<int,float> a;
    cout << "A out : " << a.sum(4,4.3) << endl;
    return 0;
} // end - main()
