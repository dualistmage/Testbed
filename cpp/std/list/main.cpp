/// 
/// @brief an example of std::list.
/// @author Dohyun Yun (dualistmage@gmail.com)
///

#include <iostream>
#include <list>
#include <map>

using namespace std;

int main()
{
    { // Test return value of size() and back()
        map< unsigned int , list<int> > a;
        list<int>& test(a[13]), &test2(a[15]);
        test2.push_back(3);
        printf("size() value when list a is empty : %u:%u\n", test.size(), a[13].size());
        printf("addr   value when list a is empty : %x:%x\n", &(a[13]), &test);
        printf("size() value when list a is empty : %u:%u\n", test2.size(),a[15].size());
        printf("addr   value when list a is empty : %x:%x\n", &(a[15]), &test2);
    }
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
