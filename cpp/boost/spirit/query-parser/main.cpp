#include <iostream>

#include "QueryParser.h"

using namespace std;
using namespace sf1v5;

int main()
{
    QueryTreePtr queryTree(new QueryTree());

    std::string queryString;
    while(getline(cin, queryString))
    {
        if ( queryString.empty() )
            break;
        if ( QueryParser::parseQuery(queryString, queryTree) )
            queryTree->print();
        else
            cout << "Parsing Fail" << endl;
    }
} // end - main()
