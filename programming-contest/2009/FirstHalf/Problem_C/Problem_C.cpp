#include <iostream>
#include <fstream>
#include <vector>
#include <map>

//#define DEBUG
//#define DEBUG2

using namespace std; 


// //////////////////////////////////////////////////
int main(int argc, char** argv)
{

    // ----------------------------------- [ check execution parameters]
    if (argc != 4)
    {
        cout << "USage : Problem_C <dic_file> <input_file> <output_file>\n";
        return -1;
    } // end - if

    // ----------------------------------- [ Open input file ]
    
    /***
    ifstream fpin;
    ofstream fpout;

    fpin.open(argv[1]);
    if (!fpin.is_open()) {
        cout << argv[1] << " is not opened" << endl;
        return -1;
    } // end - if

    string dic, word;

    // Get dic
    while(fpin.good())
    {
        fpin >> word;
        
        if (!fpin.good())
            break;

        dic += word;
    } // end - while

    fpin.close();

    fpin.open(argv[2]);
    if (!fpin.is_open()) {
        cout << argv[2] << " is not opened" << endl;
        return -1;
    } // end - if

    // Process each testcase
    

    // Variable Set -----------------------
    string pat;
    int k, weight;
    size_t i, j;
    size_t dicLength = dic.length(), patLength; 
    // Variable Set -----------------------



    int **SE;

    SE = (int**)malloc( 101 * sizeof(int*));
    for(i = 0; i < 101; i++)
        SE[i] = (int*)malloc( dicLength * sizeof(int) );

    vector<string> result;

    while(fpin.good())
    {
        fpin >> pat >> k;
        
        if (!fpin.good())
            break;

        // build SE table
        patLength = pat.length(); 
        for(i = 0; i <= patLength; i++)
            SE[i][0] = i;
        for(j = 1; j <= dicLength; j++)
            SE[0][j] = 0;
        for(i = 1; i <= patLength; i++)
        {
            for(j = 1; j <= dicLength; j++)
            {
                if ( dic[i-1] == pat[j-1] )
                    weight = 0;
                else
                    weight = 1;
                SE[i][j] = min( SE[i][j-1] + 1, min( SE[i-1][j] + 1, SE[i-1][j-1] + weight) );
            } // end - for
        } // end - for


    } // end - while


    for(i = 0; i < 101; i++)
        delete SE[i];
    delete [] SE;


    fpout.open(argv[3]);

    fpin.close();
    fpout.close();
    ***/

    return 0;

} // end - main
