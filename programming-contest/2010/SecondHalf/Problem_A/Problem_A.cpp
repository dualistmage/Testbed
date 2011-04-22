///
/// @file   Problem_A.cpp
/// @author Dohyun Yun
/// @date   2010.10.02
///

#include "CommonHead.h"
#include "TermGenerator.h"
#include "svd.h"
#include "d-mat2d.h"

using namespace std;
const string WORD_DELIMITER_STRING = " `~!@#$%^&*()_-+={}[]\\|\"':;?/>.<,\n\r";

int main(int argc, char* argv[] )
{
    // ----------------------------------- [ Global Variables ]
    TermGenerator tg(WORD_DELIMITER_STRING);
    boost::unordered_set<std::string> dupSentenceSet;
    boost::unordered_map<ID_T, std::list<ID_T> > sentenceList; // sen ID - term id list of that sentence



    // ----------------------------------- [ check execution parameters]
    if (argc != 3)
    {
        cout << "USage : Problem_A <input_file> <output_file>\n";
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

    for(size_t i = 0; i < n; i++)
    {
        double sentenceScore = 0.00;
        for(size_t j = 0; j < n; j++)
        {
            if (i == j) 
                continue;
            sentenceScore += mat_dot_product(X, k, n, i, j);
        }
        tg.insertSentenceScore(i, sentenceScore);
    }


    // ----------------------------------- [ Write top 10 sentences into file.
    fpout.open(argv[2]);
    std::vector<ID_T> topSID;
    std::vector<std::string> topSStr;
    tg.getTopSentences(topSID, topSStr);
    for(size_t i = 0; i < 10; i++)
        fpout << "["<< topSID[i]<< "] " << topSStr[i] << endl;
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

