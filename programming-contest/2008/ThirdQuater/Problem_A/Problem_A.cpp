#include <iostream>
#include <fstream>
#include <string>
#include <map>

using namespace std; 



bool compare (string first, string second)
{
    unsigned int i=0;
    while ( (i<first.length()) && (i<second.length()) )
    {
        if (first[i] < second[i]) return true;
        else if (first[i] > second[i]) return false;
        ++i;
    }
    if (first.length() < second.length()) return true;
    else return false;
}


int main(int argc, char** argv)
{


    ifstream fpin;
    ofstream fpout;
    int testCaseNo;
    int thisTest;

    int i, k, N;
    int* c;
    string* characterList;
    string P;

    map<string, string> permutationList;
    map<string, string>::iterator iter;
    pair<map<string, string>::iterator, bool> ret;



    // ----------------------------------- [ check execution parameters]
    if (argc != 3)
    {
        cout << "USage : Problem_A <INPUT> <OUTPUT>\n";
        return -1;
    } // end - if

    // ----------------------------------- [ Open input file ]
    fpin.open(argv[1]);
    if (!fpin.is_open()) {
        cout << argv[1] << " is not opened" << endl;
        return -1;
    } // end - if

    fpin >> testCaseNo;

    characterList = new string[testCaseNo];

    for( i = 0; i < testCaseNo; i++ )
    {
        fpin >> characterList[i];
    } // end - for

    fpin.close();

    c = NULL;

    thisTest = 0;

    fpout.open(argv[2]);
    // ----------------------------------- [ find permutation for each case ]
    while(thisTest < testCaseNo) {
        
        // Initialization
        permutationList.clear();

        if (c != NULL)
            delete c;

        P = characterList[thisTest];
        
        i = N = characterList[thisTest].size();
        c = new int[N + 1];

        memset(c, 0, sizeof(int)* (N+1));

        // Generate permutation
        while(1) 
        {
            c[i] = 1;
            if (i <= 2) 
            {
                break;
            }

            i--;
        }

        permutationList.insert(pair<string, string>(P,P));

        do
        {
            if (c[i] < i) {

                if (i%2 == 1) 
                    k = 1;
                else k = c[i];

                swap(P[i-1], P[k-1]);
                c[i] += 1;
                i = 2;

                permutationList.insert(pair<string, string>(P,P));

            } // end - if

            else {
                c[i] = 1;
                i += 1;
            } // end - if

        } while(i <= N);

/*
    // ----------------------------------- [ Eliminating duplicated words ]
        iter = permutationList.begin();
        string dupCheck = *iter; 
        iter++;

        for(;iter != permutationList.end();)
        {

            if (!compare(dupCheck, *iter))
                iter = permutationList.erase(iter);
            else {
                dupCheck = *iter;
                iter++;
            }
        }
*/


    // ----------------------------------- [ Output result ]
        fpout << permutationList.size() << endl;
        for(iter = permutationList.begin(); iter != permutationList.end(); iter++)
            fpout << (*iter).first << endl; 
        thisTest++;
    } // end - while() 

    fpout.close();

    return 0;

} // end - main
