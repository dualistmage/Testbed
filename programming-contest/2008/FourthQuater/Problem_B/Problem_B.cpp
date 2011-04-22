#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>

using namespace std;

struct ScoreList
{
    int index_;
    double score_;
};

int compare(const void*a, const void*b)
{
    ScoreList *x,*y;
    x = (ScoreList*)a;
    y = (ScoreList*)b;

    if ( x->score_ < y->score_ )
        return 1;  // y should be first
    else if ( x->score_ > y->score_ )
        return -1;   // x should be first
    else if ( x->index_ > y->index_ )
        return 1;  // small index should be first
    else 
        return -1;
}

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        cout << "[Usage] Problem_B <input_file> <output_file>" << endl;
        return -1;
    }

    // Read input file
    ifstream fpin(argv[1]);
    ofstream fpout(argv[2]);

    if (!fpin.is_open())
    {
        cout << "file open error" << endl;
        return -2;
    } // end - if


    // get data from input file
    size_t i, j;
    int caseNo = 0;

    char ch;
    char num[100];
    double dNum;
    size_t displayNum;
    int index;

    vector<double> targetSequence;
    vector<double> patternSequence;
    ScoreList scoreList[10100];

    fpin >> caseNo;

    for(size_t caseNum = 0; caseNum < (size_t)caseNo; caseNum++)
    {
        // Read target sequence
        index = 0;
        targetSequence.clear();
        patternSequence.clear();

        fpin.get(ch);

        while(1)
        {
            fpin.get(ch);

            if (ch == '\n')
            {    
                num[index] = '\0';
                sscanf(num, "%lf", &dNum);
                targetSequence.push_back(dNum);
                break;
            }
             
            else if (ch == ',')
            {
                num[index] = '\0';
                sscanf(num, "%lf", &dNum);
                targetSequence.push_back(dNum);
                index = 0;
            }
            else
            {
                num[index++] = ch;
            }
        }

        // Read target sequence
        index = 0;
        while(1)
        {
            fpin.get(ch);

            if (ch == '\n')
            {    
                num[index] = '\0';
                sscanf(num, "%lf", &dNum);
                patternSequence.push_back(dNum);
                break;
            }
            else if (ch == ',')
            {
                num[index] = '\0';
                sscanf(num, "%lf", &dNum);
                patternSequence.push_back(dNum);
                index = 0;
            }
            else
            {
                num[index++] = ch;
            }
        } // end - while

        // Read display num
        fpin >> displayNum;


        /*/debug
        cout << "Test " << caseNum << endl;
        for(size_t s = 0; s < targetSequence.size(); s++)
            cout << targetSequence[s] << " ";
        cout << endl;

        for(size_t s = 0; s < patternSequence.size(); s++)
            cout << patternSequence[s] << " ";
        cout << endl;

        cout << "top : " << displayNum << endl;
        //*/

        // Calculate similarity
        size_t targetLength = targetSequence.size();
        size_t patternLength = patternSequence.size();
        double u, d1, d2;
        double t, p;
        
        for(i = 0; i < targetLength - patternLength + 1; i++)
        {
            scoreList[i].index_ = i;
            
            u = d1 = d2 = 0;
            for(j = 0; j < patternLength ; j++)
            {
                t = targetSequence[i + j];
                p = patternSequence[j];

                u += t * p ;
                d1 += t*t;
                d2 += p*p; 

                /*/debug
                cout << "t (" << t << ") ";
                cout << "p (" << p << ") ";
                cout << "u (" << u << ") ";
                cout << "d1 (" << d1 << ") ";
                cout << "d2 (" << d2 << ") ";
                cout << endl;
                //*/
            }

            d1 = sqrt(d1);
            d2 = sqrt(d2);
            scoreList[i].score_ = u / (d1*d2);
        }

        // sort scorelist
        qsort( scoreList, targetLength - patternLength + 1, sizeof(ScoreList), compare );

        /*/debug 
        //
        for(i = 0; i < targetLength - patternLength + 1; i++)
        {
            cout << scoreList[i].score_ << " " << scoreList[i].index_ << endl;
        }
        //*/

        // display into output

        for(i = 0; i < displayNum; i++)
        {
            fpout << scoreList[i].index_ << endl;
        }

    } // end - for

    fpin.close();
    fpout.close();

} // end - main
