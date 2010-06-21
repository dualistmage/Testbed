#include <iostream>
#include <fstream>

#include "QueryParser.h"

using namespace std;
using namespace sf1v5;

int main()
{
    QueryTreePtr queryTree(new QueryTree());
    std::string queryString;

    {
        ifstream fpin("testset");
        std::string input, output, answer;
        while( fpin.good() )
        {
            getline( fpin, input );
            getline( fpin, answer );
            if ( ! fpin.good() )
                break;
            QueryParser::normalizeQuery(input, output); 
            if ( output != answer )
            {
                cout << "Test Case Fail" << endl;
                cout << "     - input  : " << input << endl;
                cout << "     - output : " << output << endl;
                cout << "     - answer : " << answer << endl;
            }

            if ( !QueryParser::parseQuery(output, queryTree) )
                cout << "Test Case Parsing Failed : " << output << endl;
        }
    }

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
