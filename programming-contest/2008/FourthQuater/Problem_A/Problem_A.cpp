#include <iostream>
#include <fstream>
#include <vector>
#include <map>

using namespace std; 

struct Node
{
    Node()
    {
        left = right = NULL;
        index = 0;
    }
    int index;
    struct Node* left;
    struct Node* right;
};

// Make
// //////////////////////////////////////////////////
struct Node* Make( vector<int>& freq, vector<int>& freqSum, int start, int end, struct Node* root)
{
    //cout << start << " :::::::::: " << end << endl;
    if (start == end) 
    {
        //cout << "Create " << start << endl;
        struct Node* T = new struct Node;
        T->index = start;
        T->left = T->right = NULL;
        return T;
    }
    else if (start < 0)
    {
        //cout << "Create " << end << endl;
        struct Node* T = new struct Node;
        T->index = end;
        T->left = T->right = NULL;
        return T;
    }

    else if (end >= freqSum.size())
    {
        struct Node* T = new struct Node;
        //cout << "Create " << freqSum.size() -1 << endl;
        T->index = freqSum.size() -1;
        T->left = T->right = NULL;
        return T;
    }

    // find k
    int i, k, sum, min, diff, prevdiff, index=start;
    prevdiff = min = sum = freqSum[freqSum.size()-1];

    int diff1, diff2;
    for(i = start; i < end; i++)
    {
        if (start != 0)
            diff1 = freqSum[i] - freqSum[start - 1];
        else 
            diff1 = freqSum[i];

        diff2 = freqSum[end] - freqSum[i];

        diff = abs(diff1 - diff2);

        if ( diff < min )
        {
            min = diff;
            index = i;
        }
        if (prevdiff < diff) 
            break;
        prevdiff = diff;
    } // end - for
    k = index;

    //cout << "Diff : " << k << "(" << prevdiff <<")"<< endl;

    struct Node *T1, *T2;
    struct Node* MergeTree = new struct Node;

    if (k == start) {
        //cout << "Create " << k << endl;
        MergeTree->index = k;
        MergeTree->left  = NULL;
        //cout << "Add Right" << endl;
        MergeTree->right = Make( freq, freqSum, k+1, end, root);
        return MergeTree;
    }
    else if (k == end){
        //cout << "Create " << k << endl;
        MergeTree->index = k;
        //cout << "Add left" << endl;
        MergeTree->left  = Make( freq, freqSum, k+1, end, root);
        MergeTree->right = NULL;
        return MergeTree;
    }
    else {
        //cout << "Create " << k << endl;
        MergeTree->index = k;

        //cout << "Add left" << endl;
        T1 = Make( freq, freqSum, start, k-1, root);
        MergeTree->left = T1;

        //cout << "Add Right" << endl;
        T2 = Make( freq, freqSum, k+1, end, root);
        MergeTree->right = T2;
        return MergeTree;
    }

}

void buildBinaryTree(int index, struct Node* root, string& result)
{
    result="0";

    //cout << "Build (" << index << ") ============= ";
    struct Node* tmp = root;

    while(1)
    {
        if (tmp == NULL) 
        { 
            //cout << "NULL" << endl;
            return;
        }

        //cout << tmp->index;
        if (tmp->index > index)
        {
            result += "0";
            tmp = tmp->left;
            continue;
        }
        else if (tmp->index < index)
        {
            tmp = tmp->right;
            result += "1";
            continue;
        }
        else
        {
            //cout << endl;
            return;
        }
    } // end - while()
}

// Main
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

    string keyword;
    string checkKeyword;
    vector<string> keywordList;
    map<char,int> freqMap;
    map<char,int> freqIndex;
    map<char,int>::iterator iter;


    // Get input data and Build frequency map 
    while(fpin.good())
    {
        fpin >> keyword;
        if (keyword == checkKeyword)
            break;

        for(unsigned int i=0; i < keyword.size(); i++)
        {
            iter = freqMap.find(keyword[i]);
            if (iter == freqMap.end())
            {
                freqMap[keyword[i]] = 1;
            }
            else
                freqMap[keyword[i]]++;
        } // end - for

        keywordList.push_back(keyword);

        checkKeyword = keyword;
    } // end - while


    /*/DEBUG
    //cout << "Freq----------" << endl;
    for(iter = freqMap.begin(); iter != freqMap.end(); iter++)
        //cout << iter->first << " : " << iter->second << endl;
    //cout << "keyword List" << endl;
    for(unsigned int i=0; i < keywordList.size(); i++)
        //cout << keywordList[i] << endl;
    //*/

    vector<int> freq;
    vector<int> freqSum;
    int sum = 0;
    int indexNo = 0;
    for(iter = freqMap.begin(); iter != freqMap.end(); iter++)
    {
        freq.push_back(iter->second);
        sum += iter->second;
        freqSum.push_back(sum);
        freqIndex[iter->first] = indexNo++;
    }

    /*/DEBUG
    //cout << "freqSum ====================" << endl;
    for(unsigned int i=0; i < freq.size(); i++)
        //cout << freq[i] << " : " << freqSum[i] << endl;
    //*/

    // Build Optimal alphabetic tree
    struct Node* root = new struct Node;
    root = Make( freq, freqSum, 0, (int)freq.size() - 1, root );

    string result;
    string subResult;

    /*/DEBUG
    cout << "Tree display" << endl;
    int freqDebugIndex = 0;
    for(iter = freqMap.begin(); iter != freqMap.end(); iter++)
    {
        buildBinaryTree(freqDebugIndex++, root, result);
        cout << freqDebugIndex -1 << " " << iter->first << " " << result << endl;
    }
    cout << "Tree display" << endl;
    //*/


    // display result
    for(unsigned int i=0; i < keywordList.size(); i++)
    {
        result = "";
        for(unsigned int j = 0; j < keywordList[i].size(); j++)
        {
            buildBinaryTree(freqIndex[keywordList[i][j]], root, subResult);
            result += subResult;
        }

       fpout << result <<": "<< keywordList[i] << endl;
    } // end - for
    return 0;

} // end - main
