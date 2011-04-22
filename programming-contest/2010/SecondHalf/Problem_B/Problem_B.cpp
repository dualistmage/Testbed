///
/// @file   Problem_B.cpp
/// @author Dohyun Yun
/// @date   2010.10.02
///

#include "CommonHead.h"
#include "TermGenerator.h"
#include "svd.h"
#include "d-mat2d.h"
#include <values.h>

using namespace std;
const string WORD_DELIMITER_STRING = " `~!@#$%^&*()_-+={}[]\\|\"':;?/>.<,\n\r";

double euDist(double** X, size_t i, size_t j, size_t size)
{
    if (i == j)
        return 0.001;

    double prevSum = 0.00;
    double diff;
    for(size_t k = 0; k < size; k++)
    {
        diff = X[k][i] - X[k][j];
        prevSum += diff * diff;
    }
    prevSum = sqrt(prevSum);

    if ( prevSum == 0.00 ) 
        return 0.001;

    return prevSum;
}

double uij(double** X, std::vector<ID_T>& V, size_t i, size_t j, size_t c, size_t size)
{
    double child = 1 / euDist( X, j, V[i], size );

    double root = 0.00;
    for(size_t k = 0; k < c; k++)
        root += 1 / euDist( X , j , V[k], size );

    return child / root;
}

int main(int argc, char* argv[] )
{
    // ----------------------------------- [ Global Variables ]
    TermGenerator tg(WORD_DELIMITER_STRING);
    boost::unordered_set<std::string> dupSentenceSet;
    boost::unordered_map<ID_T, std::list<ID_T> > sentenceList; // sen ID - term id list of that sentence



    // ----------------------------------- [ check execution parameters]
    if (argc != 3)
    {
        cout << "USage : Problem_B <input_file> <output_file>\n";
        return -1;
    } // end - if

    // ----------------------------------- [ Open input file ]
    std::ifstream fpin;
    std::ofstream fpout;

    fpin.open(argv[1]);
    if (!fpin.is_open()) {
        cout << argv[1] << " is not opened" << endl;
        return -1;
    } // end - if


    // ----------------------------------- [ Read input file and build sentence list ]
    while( fpin.good() )
    {
        std::string curSentence;
        getline(fpin, curSentence);

        if ( !fpin.good() )
            break;

        // Skip Duplicated string
        if ( dupSentenceSet.find(curSentence) != dupSentenceSet.end() )
            continue;

        // Insert new Sentence to the dup dic. 
        dupSentenceSet.insert(curSentence);

        std::list<ID_T> sentenceTermIdList;
        ID_T sentenceId = tg.getTermIdList( curSentence, sentenceTermIdList );

        sentenceList.insert( make_pair(sentenceId,sentenceTermIdList) );
    }
    fpin.close();

    // ----------------------------------- [ Generate tf Matrix
    size_t m = tg.getTotalTermNum(), n = tg.getTotalSentenceNum();
    size_t k = 50;

    double** tf = new double*[m];
    for(size_t i = 0; i < m; i++)
    {
        tf[i] = new double[n];
        memset(tf[i], 0, sizeof(double)*n);
    }

    double **U = mat_alloc(m,n);
    double **S = mat_alloc(1,n);
    double **V = mat_alloc(n,n);


    // Build TF Matrix
    for(boost::unordered_map<ID_T, std::list<ID_T> >::iterator senIter = sentenceList.begin(); senIter != sentenceList.end(); senIter++)
        for(std::list<ID_T>::iterator termIter = senIter->second.begin(); termIter != senIter->second.end(); termIter++)
            tf[*termIter][senIter->first]++;

    // ----------------------------------- [ Calculate score of sentences
    mat_svd(tf, m, n, U, S[0], V);

    double **US = mat_alloc(k,k);
    double **X  = mat_alloc(k,n);
    for(size_t j = 0; j < k; j++)
        for(size_t i = 0; i < k; i++)
            US[i][j] =  U[i][j] * S[0][j];

    for( size_t j = 0; j < n; j++)
    {
        for(size_t i = 0; i < k; i++) {
            double localScore = 0;
            for( size_t sk=0; sk < k; sk++)
                localScore += US[i][sk] * V[j][sk];
            X[i][j] = localScore;
        }
    }


    // ----------------------------------- [ Build u(c,n) and V(k,n) of FCMdd algorithm
    size_t c = 10;
    size_t iter = 0;
    size_t MAX_ITER = 100;
    double **u = mat_alloc(c,n);
    std::vector<ID_T> Vold(c),Vnew(c);

    // Initialization of V
    for(size_t i = 0; i < c; i++)
        Vnew[i] = i;

    while(1) {
        for(size_t i = 0; i < c; i++)
            for(size_t j = 0; j < n; j++)
                u[i][j] = uij(X, Vnew, i, j, c, k);

        Vold = Vnew;

        for(size_t i = 0; i < c; i++)
        {
            int minK = -1;
            double minSum = 0.00;
            for(size_t j = 0; j < n; j++)
                minSum += u[i][j] * euDist( X, minK, j , k );

            tg.clearSentenceScore();

            for(size_t kb = 0; kb < n; kb++)
            {
                { // Skip computing of previous medoid sentence
                    bool duplicated = false;
                    for(size_t di = 0; di < i; di++)
                    {
                        if ( Vnew[di] == kb )
                        {
                            duplicated = true;
                            break;
                        }
                    }
                    if ( duplicated )
                        continue;
                }

                double localSum = 0.00;
                for(size_t j = 0; j < n; j++)
                    localSum += u[i][j] * euDist( X, kb, j , k );
                if ( localSum < minSum )
                {
                    minK = kb;
                    minSum = localSum;
                }
            }

            Vnew[i] = minK;
        }

        iter++;
        if ( Vold == Vnew || iter == MAX_ITER )
            break;
    } 


    // ----------------------------------- [ Write top 10 sentences into file.
    fpout.open(argv[2]);
    std::vector<ID_T> topSID;
    std::vector<std::string> topSStr;
    tg.getTopSentences(topSID, topSStr);
    for(size_t i = 0; i < 10; i++)
        fpout << "["<< Vnew[i]<< "] " << tg.sentenceIdStrMap_[Vnew[i]] << endl;
    fpout.close();

    
    // ----------------------------------- [ Deallocate variables
    for(size_t i = 0; i < m; i++)
        delete[] tf[i];
    delete tf;

    mat_free(U);
    mat_free(S);
    mat_free(V);
    mat_free(US);
    mat_free(X);


    return 0;

} // end - main()

