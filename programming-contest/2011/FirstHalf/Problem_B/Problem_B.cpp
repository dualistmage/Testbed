///
/// @file   Problem_B.cpp
/// @author Dohyun Yun
/// @date   2010.01.09
///

#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char* argv[] )
{
    if ( argc != 4 )
    {
        cout << "[Usage] Problem_B <train> <testdata> <output>" << endl;
        return 0;
    } // end - if

    // ---------------------------------------- [ Read Documents
    bool ret;
    DocSet docSet;
    ret = DocSet::readFromFile( argv[1] , docSet );
    if ( ret == false )
    {
        cerr << "[Error] fail to load " << argv[1] << endl;
        return -1;
    }

    // ---------------------------------------- [ Training
    std::set<ClassID> classIdSet( docSet.getAllClasses() );
    std::vector<ClassID> classIdVec( classIdSet.begin() , classIdSet.end() );
    NBMAP_T nbmap;

    size_t setSize = classIdVec.size();
    for(size_t i = 0; i < setSize - 1; i++)
    {
        for( size_t j = i + 1; j < setSize ; j++ )
        {
            ClassID left = classIdVec[i];
            ClassID right = classIdVec[j];
            NBClassifier* obj( new NBClassifier(left, right) );
            obj->train( docSet );
            nbmap.insert( make_pair( make_pair(i,j), obj ) );
        }
    }

    // ---------------------------------------- [ Read test documents
    DocSet testDocSet;
    ret = DocSet::readFromFile( argv[2] , testDocSet );
    if ( ret == false )
    {
        cerr << "[Error] fail to load " << argv[2] << endl;
        return -1;
    }

    // ---------------------------------------- [ Predict 
    int testDocSize = testDocSet.getDocCount();
    for(int i = 0; i < testDocSize; i++)
    {
        DocVector& curDoc( testDocSet.getDocA(i) );
        size_t left = 0, right = setSize-1;
        while( left != right )
        {
            ClassID leftId = classIdVec[left];
            ClassID curId = nbmap[ make_pair(left,right) ]->predict( curDoc );
            if ( curId != leftId )
                left++;
            else
                right--;
        }
        curDoc.setClassID( classIdVec[left] );
    }
    
    // ---------------------------------------- [ Write file
    ret = DocSet::writeToFile( argv[3] , testDocSet );
    if ( ret == false )
    {
        cerr << "[Error] fail to write" << argv[3] << endl;
        return -1;
    }

    // ---------------------------------------- [ Release memory
    for( NBMAP_T::iterator iter = nbmap.begin(); iter != nbmap.end(); iter++ )
    {
        delete iter->second;
        iter->second = NULL;
    }
}
