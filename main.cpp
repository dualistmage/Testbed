#include <iostream>
#include <wiselib/ustring/UString.h>

using std::cout;
using std::endl;

int main(int argc, char* argv[])
{
                    //  0123 45 67 8
    std::string sample("abc가A나1다/");
    wiselib::UString ustr(sample, wiselib::UString::UTF_8);

    for(size_t i = 0; i < ustr.length(); i++)
    {
        if ( ustr.isKoreanChar(i) )
            cout << i << " : Korean " << endl;
        else if ( ustr.isAlphaChar(i) )
            cout << i << " : English" << endl;
        else 
            cout << i << " : Else" << endl;
    }
}
