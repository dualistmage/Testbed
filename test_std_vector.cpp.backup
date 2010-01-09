#include <iostream>
#include <vector>

using namespace std;

int main()
{
    { // Erase
        std::vector<int> t;
        t.push_back(1);
        t.push_back(2);
        t.push_back(3);
        t.push_back(4);
        t.push_back(5);
        t.push_back(6);


        if ( t.size() > 5 )
        t.erase( t.begin() + 5, t.end() );
        cout << "t size : " << t.size() << " = ";
        for(size_t i = 0; i < t.size(); i++)
            cout << " " << t[i];
        cout << endl;

        t.erase( t.begin() + 3, t.end() );
        cout << "t size : " << t.size() << " = ";
        for(size_t i = 0; i < t.size(); i++)
            cout << " " << t[i];
        cout << endl;

        t.erase( t.begin() + 10, t.end() ); // without size check
        cout << "t size : " << t.size() << " = ";
        for(size_t i = 0; i < t.size(); i++)
            cout << " " << t[i];
        cout << endl;
    }

}
