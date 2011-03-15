///
/// @brief test code of c++ function pointer
/// @author Dohyun Yun ( dualistmage@gmail.com )
/// @date 2010-06-14
///


#include <iostream>

#include "function-pointer.h"

using std::cout;
using std::endl;

void test2(void* input, void* output)
{
    output = input;
}

int main(int argc, char* argv[])
{
    { // TEST1 : Test function pointer
        ParentClass pc;
        Child1 child1;
        Child2 child2;

        pc.run(1,'c');
        pc.copyCallback(child1);
        pc.run(1,'c');
        pc.copyCallback(child2);
        pc.run(1,'c');
    }
    {
        char input[20] = "hello";
        void* output = 0x00;

        test2( (void*)input, output );
        printf("input : %x output : %x\n", input, output);
    }
}
