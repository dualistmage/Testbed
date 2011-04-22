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
    if (argc != 3)
    {
        cout << "USage : Problem_B <input_file> <output_file>\n";
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
    size_t i, j, firstLength, secondLength, longLength;
    int edit[101][101], weight;
    int path[101][101];
    char resultString[2][101];
    int a,b,c;
    int x, y, firstIndex, secondIndex, resultIndex;
    bool flag;

    // Get input data and Build frequency map 
    while(fpin.good())
    {
        fpin >> firstString >> secondString;
        
        if (!fpin.good())
            break;

        //cout << firstString << " " << secondString << endl;
        firstLength = firstString.length();
        secondLength = secondString.length();

        if ( firstLength == secondLength )
        {
            fpout << endl;
            fpout << firstString[0];
            for(i = 1; i < firstLength; i++)
                fpout << ' ' << firstString[i];

            fpout << endl << secondString[0];
            for(i = 1; i < secondLength; i++)
                fpout << ' ' << secondString[i];
            fpout << endl;
            continue;
        }
        else if ( firstLength > secondLength )
        {
            flag = true;
            swap( firstString, secondString );
            swap( firstLength, secondLength );
        }
        else 
            flag = false;


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
                edit[i][j] = min( min( edit[i-1][j] + 1, edit[i][j-1] + 1), edit[i-1][j-1] + weight);

                /************************************* ##
                a = edit[i-1][j] + 1;
                b = edit[i][j-1] + 1;
                c = edit[i-1][j-1] + weight;


                // a : delete first       2
                // b : delete second      2
                // c : w == 1 just go     1
                // c : w == 0 replace     3

                if ( a > b )
                {
                    if ( b > c ) // c is min
                    {
                        edit[i][j] = c;
                        path[i][j] = 7; // upper left
                    }
                    else if ( b == c )
                    {
                        if ( weight == 1 )
                        {
                            edit[i][j] = c;
                            path[i][j] = 7; // upper left

                        } // end - if
                        else 
                        {
                            edit[i][j] = b;
                            path[i][j] = 4; // left
                        } // end - else
                    }
                    else  // b is min
                    {
                        edit[i][j] = b;
                        path[i][j] = 4; // left
                    }
                } 
                else if ( a > c ) // c is min
                {
                    edit[i][j] = c;
                    path[i][j] = 7; // upper left
                }
                else if ( a == c )
                {
                    if ( weight == 1 )
                    {
                        edit[i][j] = c;
                        path[i][j] = 7; // upper left
                    }

                    else
                    {
                        edit[i][j] = a;
                        path[i][j] = 8; // up
                    }
                }
                else 
                {
                    edit[i][j] = a;
                    path[i][j] = 8; // up
                }
                ************************************##**/

            }
        }// end - for
        
#ifdef DEBUG2
        printf("\n\n Edit =============================================\n");
        for(i = 0; i <= firstLength; i++)
        {
            for(j = 0; j <= secondLength; j++)
            {
                printf("%3d", edit[i][j]);
            }
            printf("\n");
        }
#endif //DEBUG2

        // Recode the path
        
        memset(resultString[0], 0, sizeof(char) * 101);
        memset(resultString[1], 0, sizeof(char) * 101);

        longLength = max( firstLength, secondLength );
        resultIndex = longLength - 1 ;
        i = firstLength; j = secondLength;

        while( i > 0 && j > 0 )
        {
            a = edit[i-1][j];
            b = edit[i][j-1];
            c = edit[i-1][j-1];

#ifdef DEBUG
cout << "resultIndex : " << resultIndex << endl;
cout << endl << "Step------------------------------------------" << endl;
cout << "current i , j : " << i << " " << j << endl;
cout << "a , b , c : "<< a << " " << b << " " << c << endl;
#endif
            
            if ( a < b )
            {
//                if ( a >= c ) // Deletion
//                {
                    resultString[0][resultIndex] = firstString[i-1];
                    resultString[1][resultIndex] = secondString[j-1];

#ifdef DEBUG
cout << " both is inserted" << endl;
cout << "result[0][" << resultIndex << "] = " << resultString[0][resultIndex] << endl;
cout << "result[1][" << resultIndex << "] = " << resultString[1][resultIndex] << endl;
#endif
                    i--; j--; resultIndex--;
                    continue;
//                }
/*
                else // Delete first string
                {
                    
                    resultString[0][resultIndex] = firstString[i-1];
                    resultString[1][resultIndex] = '-';
#ifdef DEBUG
cout << " first is inserted" << endl;
cout << "result[0][" << resultIndex << "] = " << resultString[0][resultIndex] << endl;
cout << "result[1][" << resultIndex << "] = " << resultString[1][resultIndex] << endl;
#endif
                    i--; resultIndex--;
                    
                    continue;
                }
                */
            } // end - else if
            else if ( b < c && resultIndex > i -1 ) // Delete second string
            {
                resultString[0][resultIndex] = '-';
                resultString[1][resultIndex] = secondString[j-1];
#ifdef DEBUG
cout << " second is inserted" << endl;
cout << "result[0][" << resultIndex << "] = " << resultString[0][resultIndex] << endl;
cout << "result[1][" << resultIndex << "] = " << resultString[1][resultIndex] << endl;
#endif
                j--; resultIndex--;
                continue;
            }
            else // Replace
            {
                resultString[0][resultIndex] = firstString[i-1];
                resultString[1][resultIndex] = secondString[j-1];
#ifdef DEBUG
cout << " both is inserted" << endl;
cout << "result[0][" << resultIndex << "] = " << resultString[0][resultIndex] << endl;
cout << "result[1][" << resultIndex << "] = " << resultString[1][resultIndex] << endl;
#endif
                i--; j--; resultIndex--;
                continue;
            }
        } // end - while

        if ( i != 0  && j == 0 ) // add rest of first string
        {

#ifdef DEBUG
cout << "First character left : " << i << endl;
#endif

            while(i > 0)
            {
                resultString[0][resultIndex] = firstString[i-1];
                resultString[1][resultIndex] = '-';
                i--; resultIndex--;
            }
        } // end - if
        else if ( i == 0 && j != 0 )
        {

#ifdef DEBUG
cout << "Second character left : " << j << endl;
#endif

            while(j > 0)
            {
                resultString[0][resultIndex] = '-';
                resultString[1][resultIndex] = secondString[j-1];
                j--; resultIndex--;
            }
        } // end - if


        if ( flag )
        {
            fpout << endl;
            fpout << resultString[1][0];
            for(i = 1; i < longLength; i++)
                fpout << ' ' << resultString[1][i];

            fpout << endl << resultString[0][0];
            for(i = 1; i < longLength; i++)
                fpout << ' ' << resultString[0][i];
            fpout << endl;
        }
        else
        {
            fpout << endl;
            fpout << resultString[0][0];
            for(i = 1; i < longLength; i++)
                fpout << ' ' << resultString[0][i];

            fpout << endl << resultString[1][0];
            for(i = 1; i < longLength; i++)
                fpout << ' ' << resultString[1][i];
            fpout << endl;
        }

    } // end - while

    fpin.close();
    fpout.close();

    return 0;

} // end - main
