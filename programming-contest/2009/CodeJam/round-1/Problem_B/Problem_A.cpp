#include <iostream>
#include <fstream>
#include <vector>
#include <map>

using namespace std; 


// //////////////////////////////////////////////////
int main(int argc, char** argv)
{

    // ----------------------------------- [ check execution parameters]
    if (argc != 3)
    {
        cout << "USage : Problem_A <input_file> <output_file>\n";
        return -1;
    } // end - if

    // ----------------------------------- [ Open input file ]
    
    ifstream fpin;
    ofstream fpout;

    fpin.open(argv[1]);
    if (!fpin.is_open()) {
        cout << argv[1] << " is not opened" << endl;
        return -1;
    } // end - if

    fpout.open(argv[2]);

    string firstString, secondString;
    size_t i, j, firstLength, secondLength;
    int edit[101][101], weight;

    // Get input data and Build frequency map 
    while(fpin.good())
    {
        fpin >> firstString >> secondString;
        
        if (!fpin.good())
            break;

        //cout << firstString << " " << secondString << endl;
        firstLength = firstString.length();
        secondLength = secondString.length();

        // Init edit array
        for(i = 0; i <= firstLength; i++)
            edit[i][0] = i;
        for(j = 1; j <= secondLength; j++)
            edit[0][j] = j;
        for(i = 1; i <= firstLength; i++)
        {
            for( j = 1; j <= secondLength; j++)
            {
                if (firstString[i-1] == secondString[j-1])
                    weight = 0;
                else 
                    weight = 1;
                edit[i][j] 
                    = min( min( edit[i-1][j] + 1, edit[i][j-1] + 1), edit[i-1][j-1] + weight);
            }
        }// end - for
        
        fpout << edit[firstLength][secondLength] << endl;

    } // end - while

    fpin.close();
    fpout.close();

    return 0;

} // end - main
