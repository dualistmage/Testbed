///
/// @file   TermGenerator.h
/// @author Dohyun Yun
/// @date   2010.10.02
///

#ifndef _TERM_GENERATOR_H_
#define _TERM_GENERATOR_H_

#include "CommonHead.h"
#include <list>

class TermGenerator
{
    public: // typedefs

    public: // public interfaces
        TermGenerator(const std::string& delimiters);

        ///
        /// @brief generates term ids by looking at str and return sentence id also.
        /// @details
        /// - Term : word which has more than 3 characters.
        /// @param str          input string
        /// @param termIdList   Term id list.
        /// @return sentenceId
        ///
        ID_T getTermIdList(const std::string& str, std::list<ID_T>& termIdList);

        size_t getTotalTermNum() { return newTermId_; }
        size_t getTotalSentenceNum() { return newSentenceId_; }

        void insertSentenceScore(ID_T sentenceId, double score);
        void getTopSentences(std::vector<ID_T>& topSentenceId, std::vector<std::string>& topSentenceStr);



    private:
        bool isWord(const std::string& str);
        ID_T getTermId(const std::string& str);
        ID_T getSentenceId(const std::string& str);
        ID_T getId(IDMAP_T& idMap, ID_T& newId, const std::string& str);

    private: // private variables
        SEP_T separator_;
        IDMAP_T termIdMap_;
        IDMAP_T sentenceIdMap_;
        ID_T newSentenceId_;
        ID_T newTermId_;
        boost::unordered_map<ID_T, std::string> sentenceIdStrMap_;
        std::list<double> rankedSentenceScore_;
        std::list<ID_T> rankedSentenceId_;
}; // end - class TermGenerator

#endif // _STRING_TOOLS_H_

