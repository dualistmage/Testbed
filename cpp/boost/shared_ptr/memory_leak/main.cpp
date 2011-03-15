#include <iostream>
#include <list>
#include <boost/shared_ptr.hpp>

using namespace std;
using namespace boost;

class Leak;
typedef shared_ptr<Leak> LeakPtr;

class Leak
{
    public:
        Leak(int no):no_(no) {}
        void print(int tab = 0) 
        {
            for(int i = 0; i < tab; i++)
                cout << " ";
            cout << "[ " << no_ << " ]" << endl;
            for(std::list<LeakPtr>::iterator iter = children_.begin();
                    iter != children_.end(); iter++)
                (*iter)->print(tab + 3);
        }

        int no_;
        std::list<LeakPtr> children_;
};

void swap2(LeakPtr& src)
{
    LeakPtr test2( new Leak(4) );
    LeakPtr test3( new Leak(5) );
    LeakPtr test4( new Leak(6) );
    test2->children_.push_back( test3 );
    test2->children_.push_back( test4 );
    test2.swap(src);
}

int main(int argc, char* argv[])
{
    LeakPtr test1( new Leak(1) );
    LeakPtr test2( new Leak(2) );
    LeakPtr test3( new Leak(3) );

    test1->children_.push_back( test2 );
    test1->children_.push_back( test3 );

    cout <<  "Before swap" << endl;
    test1->print();
    swap2(test1);
    cout <<  "After swap" << endl;
    test1->print();
}
