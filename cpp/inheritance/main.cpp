///
/// @brief test code of c++ inheritance issues
/// @author Dohyun Yun ( dualistmage@gmail.com )
/// @date 2010-06-14
///


#include <iostream>

#include "virtual.h"

using std::cout;
using std::endl;

class grandma
{
    public:
        int gma_;
};

class ma : public grandma
{
    public:
        int ma_;
};

class child : public ma
{
    public:
        int ch_;
};


int main(int argc, char* argv[])
{
    { // TEST1
        child c;
        c.gma_  = 3;
        c.ma_   = 2;
        c.ch_   = 1;

        cout << "Child value : " << c.gma_ << " " << c.ma_ << " " << c.ch_ << " " << endl;
    }
    { // TEST1
        virtualClass test;
        test.run();
        virtualInheritClass test2;
        test2.run();
        
    }
}
