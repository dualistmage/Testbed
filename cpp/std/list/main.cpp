/// 
/// @brief an example of std::list.
/// @author Dohyun Yun (dualistmage@gmail.com)
///

#include <iostream>
#include <list>

using namespace std;

int main()
{
    { // Test splice
        list<int> a, b, c;
        a.push_back(1);
        a.push_back(2);
        a.push_back(3);

        b.push_back(4);
        b.push_back(5);
        b.push_back(6);
        b.push_back(7);

        c.push_back(8);
        c.push_back(9);

        cout << "Before Split : a( ";
        for(list<int>::const_iterator iter = a.begin(); iter != a.end(); iter++)
            cout << *iter << " ";
        cout << ") b( ";
        for(list<int>::const_iterator iter = b.begin(); iter != b.end(); iter++)
            cout << *iter << " ";
        cout << ") c( ";
        for(list<int>::const_iterator iter = c.begin(); iter != c.end(); iter++)
            cout << *iter << " ";
        cout << ")" << endl;

        list<int>::iterator bIter = b.begin(); bIter++; bIter++;
        a.splice(a.end(), b, b.begin(), bIter);
        a.splice(a.begin(), c);

        cout << "After Split  : a( "; for(list<int>::const_iterator iter = a.begin(); iter != a.end(); iter++)
            cout << *iter << " ";
        cout << ") b( ";
        for(list<int>::const_iterator iter = b.begin(); iter != b.end(); iter++)
            cout << *iter << " ";
        cout << ") c( ";
        for(list<int>::const_iterator iter = c.begin(); iter != c.end(); iter++)
            cout << *iter << " ";
        cout << ")" << endl;
    }

} 
