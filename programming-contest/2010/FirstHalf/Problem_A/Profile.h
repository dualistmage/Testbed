///
/// @file   Profile.h
/// @author Dohyun Yun
/// @date   2010.01.09
///

#ifndef _PROFILE_H_
#define _PROFILE_H_

#include <map>
#include <vector>
#include <string>

class TermWithFreq
{
    public:
        TermWithFreq( const std::string& term, uint64_t freq ) :
            term_( term ),
            freq_( freq ) {};

        bool operator < (const TermWithFreq& obj) const
        {
            if (freq_ < obj.freq_)
                return false;
            else if (freq_ == obj.freq_)
                if (term_.compare(obj.term_) > 0)
                    return false;
            return true;
        }
    public:
        std::string term_;
        uint64_t freq_;
}; // end - TermWithFreq

class Profile
{
    public:
        typedef std::map<std::string , uint64_t> NGRAM_MAP_T;

    public:
        Profile();
        Profile(const std::string& profileName);

        bool makeNGram( const std::string& input, size_t n);
        bool insertTerm( const std::string& input);
        void fileOut(const std::string& profilePath);

        bool loadProfileData();
        void buildIndexMap();
        uint64_t getIndex(const std::string& term) const;
        uint64_t getDistance(const Profile& profile);

    //private:
    public: // For testing
        void makeProfileData();

    public:
        std::string profileName_;
        std::string profilePath_;
        NGRAM_MAP_T ngram_;
        std::vector<TermWithFreq> data_;
        NGRAM_MAP_T indexMap_;

}; // end - Profile

namespace profile_tool 
{
    void replaceSpace(const std::string& in, std::string& out);

    void makeProfile(const std::string& trainPath, Profile& profile);

    std::string getCategoryName(Profile& documentProfile, std::vector<Profile>& categoryList);

} // end - namespace profile_tool

#endif
