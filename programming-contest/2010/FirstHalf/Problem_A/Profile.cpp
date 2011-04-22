#include "Profile.h"

#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

Profile::Profile()
{
}

Profile::Profile(const string& profileName):
    profileName_(profileName)
{
} // end - Profile()

bool Profile::makeNGram(const string& input, size_t n)
{
    size_t length = input.length();
    for(size_t idx = 0; idx < length; idx += n)
    {
        for(size_t j = idx; j < idx + n && j < length; j++)
        {
            std::string term;
            for(size_t i = 0; i < n && j + i < length; i++)
            {
                term += input[j + i];
                insertTerm( term );
            }
        }
    } // end - for
    
    makeProfileData();

    // DEBUG
    // NGRAM_MAP_T::iterator mapIter = ngram_.begin();
    // for(; mapIter != ngram_.end(); mapIter++)
    //     cout << mapIter->first << " " << mapIter->second << endl;

    return true;
}


bool Profile::insertTerm( const std::string& input)
{
    NGRAM_MAP_T::iterator mapIter = ngram_.find( input );
    if ( mapIter == ngram_.end() )
    {
        ngram_.insert(make_pair(input,1));
        return false;
    }
    else
    {
        mapIter->second++;
        return true;
    }
}

void Profile::makeProfileData()
{
    data_.clear();
    NGRAM_MAP_T::iterator mapIter = ngram_.begin();
    for(; mapIter != ngram_.end(); mapIter++)
    {
        // cout << "Insert : " << mapIter->first << "-" << mapIter->second << endl;
        data_.push_back( TermWithFreq( mapIter->first , mapIter->second ) );
    }

    // DEBUG
    // cout << "------------------------------------------------" << endl;
    // std::vector<TermWithFreq>::iterator iter = data_.begin();
    // for(; iter != data_.end(); iter++)
    // {
    //     cout << iter->term_ << " " << iter->freq_ << endl;
    // }
    // cout << "------------------------------------------------" << endl;

    std::sort( data_.begin() , data_.end() );

    // Select top 400
    if ( data_.size() > 400 )
        data_.erase( data_.begin() + 400 , data_.end() );
}

void Profile::fileOut(const string& profilePath)
{
    ofstream fpout( profilePath.c_str() );
    if ( !fpout.is_open() )
    {
        cerr << "[Error] File(" << profilePath << ") is not opened" << endl;
        return;
    } // end - if
    std::vector<TermWithFreq>::iterator iter = data_.begin();
    for(; iter != data_.end(); iter++)
        fpout << iter->term_ << " " << iter->freq_ << endl;
    fpout.close();
} // end - fileOut()

bool Profile::loadProfileData()
{
    if ( profilePath_.empty() )
        return false;

    ifstream fpin( profilePath_.c_str() );
    if ( !fpin.is_open() )
        return false;

    // DEBUG
    // cout << " Load from profile ---------------" << endl;

    while( fpin.good() )
    {
        std::string term;
        uint64_t freq;
        fpin >> term >> freq;

        if ( !fpin.good() )
            break;
        TermWithFreq data(term, freq);
        data_.push_back( data );
    }
    fpin.close();
}

void Profile::buildIndexMap()
{
    indexMap_.clear();

    uint64_t index = 0;
    std::vector<TermWithFreq>::iterator iter = data_.begin();

    for(; iter != data_.end(); iter++, index++)
    {
        // DEBUG
        // cout << "Building : " << iter->term_ << "-" << index << endl;
        indexMap_.insert( make_pair(iter->term_ , index) );
    }
} // end - buildIndexMap()

uint64_t Profile::getIndex(const std::string& term) const
{
    NGRAM_MAP_T::const_iterator iter = indexMap_.find( term );
    if ( iter == indexMap_.end() )
        return 400;
    else
        return iter->second;
} // end - getIndex()

uint64_t Profile::getDistance(const Profile& profile)
{
    uint64_t sumDist = 0;
    NGRAM_MAP_T::iterator iter = indexMap_.begin();
    for(; iter != indexMap_.end(); iter++)
    {
        uint64_t targetIndex = profile.getIndex( iter->first );
        if ( targetIndex == 400 )
            sumDist += targetIndex;
        else if ( targetIndex > iter->second )
            sumDist += (targetIndex - iter->second);
        else
            sumDist += (iter->second - targetIndex);

        // DEBUG
        // cout << "my and target index = (" << iter->first << ") " << iter->second << " - " << targetIndex << endl;
        // cout << "sumDist = " << sumDist << endl;
    }
    return sumDist;
} // end - getDistance()



namespace profile_tool {

    void replaceSpace( const string& in, string& out)
    {
        out.clear();

        for(string::const_iterator iter = in.begin(); iter != in.end(); iter++)
        {
            if ( isspace(*iter) || isdigit(*iter) )
            {
                out += '_'; iter++;
                while( iter != in.end() && (isspace(*iter) || isdigit(*iter)) )
                    iter++;
                if ( iter == in.end() )
                    break;
            }
            out += *iter;
        }
    } // end - replaceSpace()


    void makeProfile( const string& trainPath, Profile& profile)
    {
        if ( profile.loadProfileData() )
            return;
        // ---------------------------------------- [ Read train file
        ifstream fpin( trainPath.c_str() );
        if ( !fpin.is_open() )
        {
            cerr << "[Error] File (" << trainPath << ") is not opened" << endl;
        }
        string fileStr((istreambuf_iterator<char>(fpin)), istreambuf_iterator<char>());

        // ---------------------------------------- [ Process training data
        string newFileStr;
        replaceSpace( fileStr, newFileStr );
        profile.makeNGram(newFileStr, 5);
        fpin.close();
    } // end - makeProfile()

    std::string getCategoryName(Profile& documentProfile, std::vector<Profile>& categoryList)
    {
        documentProfile.buildIndexMap();

        uint64_t minDist = -1, minIndex = 0, curDist, curIndex = 0;
        for(std::vector<Profile>::iterator iter = categoryList.begin();
                iter != categoryList.end(); iter++, curIndex++)
        {
            iter->buildIndexMap();

            curDist = documentProfile.getDistance( *iter ); 

            // DEBUG
            // cout << "Cur Dist(" << iter->profileName_ << ") : " << curDist << endl;

            if ( curDist < minDist )
            {
                minDist = curDist; minIndex = curIndex;
            }
            
            // DEBUG
            // cout << "Cur dist : " << curDist << endl;

        }

        // DEBUG
        // cout << "Min dist : " << minDist << " - " <<  categoryList[minIndex].profileName_ << endl;

        if ( minDist > 0 )
            return categoryList[minIndex].profileName_;
        else
            return "";

    } // end - getCategoryName()


} // end - namespace profile_tool 
