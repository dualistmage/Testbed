///
/// @file   test_boost_thread_mutex.cpp
/// @author Dohyun Yun ( dualistmage@gmail.com )
/// @brief  This test code contains variable usage of boost::mutex and boost::thread
/// @details
///     - 1. Multi thread read and one thread write lock test.
///

#include <iostream>
#include <vector>
#include <boost/thread.hpp>

using std::cin;
using std::cerr;
using std::endl;

boost::shared_mutex sm;

static char threadCommandBox[4] = {'s' , 's' , 's' , 's'};
std::vector<std::string> threadMessage(4, "Start");

void displayStatus()
{
    printf("Thread | %10d | %10d | %10d | %10d |\n",0,1,2,3);
    printf("Status | %10c | %10c | %10c | %10c |\n", 
            threadCommandBox[0], 
            threadCommandBox[1], 
            threadCommandBox[2], 
            threadCommandBox[3] );
    printf("Msg    | %10s | %10s | %10s | %10s |\n\n",
            threadMessage[0].c_str(),
            threadMessage[1].c_str(),
            threadMessage[2].c_str(),
            threadMessage[3].c_str() );
}
void threadRun(int commandBoxIndex)
{
    assert( commandBoxIndex < 4 && 0 <= commandBoxIndex );
    std::string& msg( threadMessage[commandBoxIndex] );
    volatile char& command( threadCommandBox[commandBoxIndex] );

    bool stillRunning = true;
    int cnt = 0;

    while( stillRunning ) {
        //cerr << "[" << commandBoxIndex << "," << command << "] " << endl;;
        switch( command ) {
            case 'r' :
                {
                    boost::shared_lock<boost::shared_mutex> lock(sm);
                    msg = "Read Lock";
                    while( command != 'd' );
                    msg = "Done";
                    break;
                }
            case 'w' :
                {
                    boost::upgrade_lock<boost::shared_mutex> lock(sm);
                    boost::upgrade_to_unique_lock<boost::shared_mutex> uniqueLock(lock);
                    msg = "Write Lock";
                    while( command != 'd' );
                    msg = "Done";
                    break;
                }
            case 'f' :
                stillRunning = false;
                break;
            case 's' :
                break;
            default :
                break;
        } // end - switch
    } // end - while

    msg = "Finish";
}

int main()
{
    boost::thread_group tg;
    tg.create_thread( boost::bind(&threadRun,0) );
    tg.create_thread( boost::bind(&threadRun,1) );
    tg.create_thread( boost::bind(&threadRun,2) );
    tg.create_thread( boost::bind(&threadRun,3) );

    while(true)
    {
        int index; char cmd;
        cerr << "Command List" << endl
             << "---------------------------" << endl
             << "r = start read lock" << endl
             << "w = start write lock" << endl
             << "d = finish locking" << endl
             << "f = finish thread" << endl
             << "s = display status" << endl << endl
             << "Command (Format:index cmd, exit if index == -1) : "; 
        cin >> index >> cmd;
        if ( index == -1 ) break;
        if ( index > 3 || index < 0 )
        {
            cerr << "Wrong command type again" << endl;
            continue;
        }
        switch(cmd)
        {
            case 'r':
            case 'w':
            case 'f':
            case 'd':
                threadCommandBox[index] = cmd;
                displayStatus();
                break;
            case 's':
                displayStatus();
                break;
            default:
                cerr << "Wrong command type again" << endl;
        }
    }

    // wait until all processes are done.
    tg.join_all();

    return 0;
}

