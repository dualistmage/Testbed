#include "IPRestrictor.h"

#include <iostream>
#include <vector>

#include <boost/asio/ip/address.hpp>
#include <boost/array.hpp>

using std::cout;
using std::endl;


int main()
{
    sf1v5::IPRestrictor obj;

    obj.registerAllowIP( "1" );
    obj.registerAllowIP( "2.1" );
    obj.registerAllowIP( "2.2.1" );
    obj.registerAllowIP( "2.2.2.1" );

    obj.registerDenyIP( "3" );
    obj.registerDenyIP( "4.1" );
    obj.registerDenyIP( "4.2.1" );
    obj.registerDenyIP( "4.2.2.1" );
    obj.registerDenyIP( "2.2.2" );

    obj.registerAllowIP("77");
    obj.registerDenyIP("77.77");
    obj.registerAllowIP("77.77.77");
    obj.registerDenyIP("77.77.77.77");

    obj.registerAllowIP("77.88");
    obj.registerDenyIP("77.88");

    // allow > deny
    std::string a1("77.1.1.1");

    // allow < deny
    std::string b1("77.77.1.1");


    // allow == deny but not 0
    std::string b2("77.88.1.1");
    
    // allow == deny 0
    std::string a2("99.1.1.1");


    obj.print();

    sf1v5::IPRestrictor::IP_BYTES_TYPE testIP;

    std::string ipString;
    while( std::cin >> ipString )
    {

        if ( obj.parseIPStringToBytes( ipString, testIP ) )
        {
            if ( obj.isThisIPAllowed( testIP ) )
                cout << "IP(" << ipString << ") allowed"<< endl;
            else
                cout << "IP(" << ipString << ") denied" << endl;
        }

        else
            cout << "Parsing Failed : " << ipString << endl;
    }



}
