///
/// @file test_system_info.cpp
/// @brief Test to get system information 
/// @author Dohyun Yun ( dualistmage@nate.com )
/// 

#include <iostream>
#include <fstream>
#include <string>

#ifdef linux
#   include <sys/sysinfo.h>     // For system info
#   include <netinet/in.h>      // IP address
#   include <sys/ioctl.h>       // ioctl()
#   include <arpa/inet.h>       // inet_ntoa
#   include <net/if.h>          // struct ifconf ifreq
#   include <netinet/ether.h>   // ether_ntoa
#endif // linux

using namespace std;

int main() 
{ 
    { // Memory size
#ifdef linux
        std::string token;
        std::ifstream file("/proc/meminfo");
        while(file >> token)
        {
            if (token == "MemTotal:") {
                unsigned long mem;
                if (file >> mem)
                    std::cout << "Total Mem size : " << mem << std::endl;
            }
            file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
#endif
    } // end - Memory size

    { // System Info
#ifdef linux
        struct sysinfo sys;
        sysinfo(&sys);
        cout << "System Information"                << endl;
        cout << "-------------------------------"   << endl;
        cout << "Uptime     : " << sys.uptime       << endl;
        cout << "loads[0]   : " << sys.loads[0]     << endl;
        cout << "loads[1]   : " << sys.loads[1]     << endl;
        cout << "loads[2]   : " << sys.loads[2]     << endl;
        cout << "total  rem : " << sys.totalram     << endl;
        cout << "free   rem : " << sys.freeram      << endl;
        cout << "shared rem : " << sys.sharedram    << endl;
        cout << "buffer ram : " << sys.bufferram    << endl;
        cout << "total swap : " << sys.totalswap    << endl;
        cout << "free swap  : " << sys.freeswap     << endl;
        cout << "procs      : " << sys.procs        << endl;
        cout << "totalhigh  : " << sys.totalhigh    << endl;
        cout << "freehigh   : " << sys.freehigh     << endl;
        cout << "mem_unit   : " << sys.mem_unit     << endl;
        cout << "_f         : " << sys._f           << endl;
#endif
    } // end - System Info

    { // IP address and mac address list : This code supposes the maximum number of ethernets as 20.
        const int MAX_ETH_NO = 20;

        struct ifconf ifcfg; 
        memset(&ifcfg, 0, sizeof(ifcfg)); // memset is needed because ioctl() will fail without this line.
        ifcfg.ifc_buf = NULL;
        ifcfg.ifc_len = sizeof(struct ifreq) * MAX_ETH_NO;
        ifcfg.ifc_buf = (char*)realloc( ifcfg.ifc_buf, ifcfg.ifc_len );

        int sock = socket(AF_INET, SOCK_DGRAM, 0);
        if ( sock == -1 )
        {
            cerr << "Socket Error" << endl;
            close(sock);
            return -1;
        }
        if ( ioctl(sock, SIOCGIFCONF, &ifcfg) < 0 )
        {
            cerr << "ioctl() Error" << endl;
            return -1;
        }


        // *** Display the number of ethernet cards ***
        cout << endl << "Total Ethernet No : " << ifcfg.ifc_len / sizeof(struct ifreq) << endl;

        struct ifreq* ifr = ifcfg.ifc_req;
        for( int n = 0; n < ifcfg.ifc_len; n += sizeof(struct ifreq), ifr++ )
        {
            cout << "  " << "[eth" << n / sizeof(struct ifreq) << "] information --------------" << endl;
            cout << "  " << " - name : " << ifr->ifr_name << endl;
            struct sockaddr_in* sin = (struct sockaddr_in*) &ifr->ifr_addr;
            cout << "  " << " - IP   : " << inet_ntoa(sin->sin_addr) << "," << sin->sin_addr.s_addr << endl;
            if ( ntohl(sin->sin_addr.s_addr) == INADDR_LOOPBACK )
                cout << "  " << "          Loop Back" << endl;
            else
            { // Display mac address
                ioctl(sock, SIOCGIFHWADDR, ifr);
                struct sockaddr* sa = &( ifr->ifr_hwaddr );
                cout << "  " << " - MAC  : " << ether_ntoa( (struct ether_addr*)sa->sa_data) 
                    << " , num = ";
                unsigned char macAddress[6];
                bcopy(sa->sa_data, macAddress, 6);
                for(size_t i = 0; i < 6; i++)
                    printf("%02X ", macAddress[i] );
                cout << endl;
            }

            ioctl(sock, SIOCGIFBRDADDR, ifr);
            sin = (struct sockaddr_in *) &(ifr->ifr_broadaddr);
                cout << "  " << " - BROAD: " << inet_ntoa(sin->sin_addr) << endl;

            ioctl(sock, SIOCGIFNETMASK, ifr);
            sin = (struct sockaddr_in *) &(ifr->ifr_broadaddr);
                cout << "  " << " - NMASK: " << inet_ntoa(sin->sin_addr) << endl;

            ioctl(sock, SIOCGIFMTU, ifr);
            sin = (struct sockaddr_in *) &(ifr->ifr_broadaddr);
                cout << "  " << " - MTU  : " << inet_ntoa(sin->sin_addr) << endl;
        } // end - for


        // Free allocated memory
        free(ifcfg.ifc_buf);
        close(sock);
        ifcfg.ifc_buf = NULL;
    } // end - Mac Address list

    return 0; 
} 
