/// 
/// @brief an example of std::priority_queue
/// @author Dohyun Yun (dualistmage@gmail.com)
///

#include <iostream>
#include <queue>

using namespace std;

class IdScorePair
{
public:
    IdScorePair(int id, float score): id_(id), score_(score) {};

    bool operator<( const IdScorePair& rhs ) const
    {
        // if
        if ( this->score_ == rhs.score_ )
        {
            cout << "Same score" << endl;
            return this->id_ > rhs.id_;
        }
        return this->score_ < rhs.score_;
    }

    int id_;
    float score_;
};

int main()
{
    {
        priority_queue<IdScorePair> alignedScoreList;

        IdScorePair d1(1,1.9); alignedScoreList.push( d1 );
        IdScorePair d2(2,3.0); alignedScoreList.push( d2 );
        IdScorePair d4(4,2.9); alignedScoreList.push( d4 );
        IdScorePair d5(5,2.1); alignedScoreList.push( d5 );
        IdScorePair d3(3,2.1); alignedScoreList.push( d3 );
        IdScorePair d6(6,0.3); alignedScoreList.push( d6 );

        cout << "Test 1 Result : Aligned Score" << endl;
        while( alignedScoreList.size() > 0 )
        {
            const IdScorePair& obj(alignedScoreList.top());
            cout << obj.id_ << "] : " <<  obj.score_ << endl;
            alignedScoreList.pop();
        }
    }
} 
