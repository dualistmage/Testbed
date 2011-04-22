#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

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

    int L, D, N;
    fpin >> L >> D >> N;
    vector<string> example(D);

    for(int i=0; i < D; i++)
    {
        string ex;
        fpin >> example[i];
    }
    for(int i=0; i < N; i++)
    {
        string rule;
        fpin >> rule;
        //cout << "Rule : " << rule << endl; // XXX
        vector<vector<char> > charDicList;
        vector<char> charDic;
        bool blank = false;
        for(size_t i = 0; i < rule.length(); i++)
        {
            if ( rule[i] == '(' )
            {
                blank = true;
                continue;
            }
            else if ( rule[i] == ')' )
            {
                charDicList.push_back( charDic );
                charDic.clear();
                blank = false;
            }
            else if ( blank == true )
                charDic.push_back( rule[i] );
            else
            {
                charDic.push_back( rule[i] );
                charDicList.push_back( charDic );
                charDic.clear();
            }
        }




        // XXX
        // cout << "Dicsize : " << charDicList.size();
        // for(size_t j = 0; j < charDicList.size(); j++)
        //     cout << " , " << charDicList[j].size();
        // cout << endl;
        // XXX

        int correctNum = 0;
        size_t examListLength = example.size();
        for(size_t j = 0; j < examListLength; j++)
        {
            int rightChar = 0;
            size_t examLength = example[j].length();
            for(size_t index = 0; index < examLength; index++)
            {
                bool charCorrect = false;
                size_t dicListSize = charDicList[index].size();
                for(size_t dicIndex = 0; dicIndex < dicListSize; dicIndex++)
                {
                    if ( example[j][index] == charDicList[index][dicIndex] )
                    {
                        charCorrect = true;
                        break;
                    }
                } // end - for

                if (charCorrect == true )
                {
                    rightChar++;
                }
                else
                    break;

            } // end - for

            if (rightChar == L)
                correctNum++;

        }  // end - for
        fpout << "Case #" << i+1 << ": " << correctNum << endl;

    }

    fpin.close();
    fpout.close();

    return 0;

} // end - main
