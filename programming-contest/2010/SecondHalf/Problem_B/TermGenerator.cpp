///
/// @file   TermGenerator.cpp
/// @author Dohyun Yun
/// @date   2010.10.02
///

#include "TermGenerator.h"
#include <algorithm>

using namespace std;

TermGenerator::TermGenerator(const std::string& delimiters)
    : separator_( delimiters.c_str() ), newSentenceId_(0), newTermId_(0)
{
} // end - TermGenerator()


ID_T TermGenerator::getTermIdList(const std::string& str, std::list<ID_T>& termIdList)
{
    // Tokenize str
    TOK_T tmpTokens( str, separator_ );
    std::list<ID_T> tmpTermIdList;

    // Generate word list
    for(TOK_T::iterator tok_iter = tmpTokens.begin(); tok_iter != tmpTokens.end(); tok_iter++)
    {
        std::string lower = *tok_iter;
        std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
        if ( isWord(lower) )
            tmpTermIdList.push_back( getTermId(lower) );
    }

    tmpTermIdList.swap(termIdList);

    ID_T sid = getSentenceId(str);

    sentenceIdStrMap_.insert( make_pair(sid, str) );

    // Return Sentence Id.
    return getSentenceId(str);
} // end - getTermIdList()

bool TermGenerator::isWord(const std::string& str)
{
    if ( str.length() < 4 )
        return false;

    std::string::const_iterator iter = str.begin();

    if (!isalpha(*iter)) // first character must be a character
        return false;

    int numAlpha = 1;
    for (; iter != str.end(); iter++)
    {
        if (!isalpha(*iter))
            return false;
        numAlpha++;
    }

    if (numAlpha == 1)
        return false; // alpha character must be more than one.
    else
        return true;
}


ID_T TermGenerator::getTermId(const std::string& str)
{
    return getId(termIdMap_, newTermId_, str);
} // end - getTermId()

ID_T TermGenerator::getSentenceId(const std::string& str)
{
    return getId(sentenceIdMap_, newSentenceId_, str);
} // end - getSentenceId()

ID_T TermGenerator::getId(IDMAP_T& idMap, ID_T& newId, const std::string& str)
{
    IDMAP_T::iterator iter = idMap.find(str);
    if ( iter != idMap.end() )
        return iter->second;

    idMap.insert( make_pair(str, newId) );
    return newId++;
} // end - getId()

void TermGenerator::insertSentenceScore(ID_T sentenceId, double score)
{
    bool inserted = false;
    std::list<double>::iterator scoreIter = rankedSentenceScore_.begin();
    std::list<ID_T>::iterator idIter = rankedSentenceId_.begin();

    for(;scoreIter != rankedSentenceScore_.end(); scoreIter++, idIter++)
        if ( *scoreIter < score )
        {
            inserted = true;
            rankedSentenceScore_.insert(scoreIter, score);
            rankedSentenceId_.insert(idIter, sentenceId);
            break;
        }

    if ( !inserted )
    {
        rankedSentenceScore_.push_back(score);
        rankedSentenceId_.push_back(sentenceId);
    }

    if ( rankedSentenceScore_.size() > 10 )
    {
        rankedSentenceScore_.pop_back();
        rankedSentenceId_.pop_back();
    }
}

void TermGenerator::getTopSentences(std::vector<ID_T>& topSentenceId, std::vector<std::string>& topSentenceStr)
{
    std::vector<ID_T> tmpTSID(10);
    std::vector<std::string> tmpSStr(10);

    std::list<ID_T>::iterator idIter = rankedSentenceId_.begin();

    for(size_t i = 0;i < 10; idIter++, i++)
    {
        tmpTSID[i] = *idIter;
        tmpSStr[i].swap( sentenceIdStrMap_[*idIter] );
    }

    tmpTSID.swap(topSentenceId);
    tmpSStr.swap(topSentenceStr);
}
