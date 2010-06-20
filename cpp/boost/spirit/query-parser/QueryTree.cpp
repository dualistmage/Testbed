///
/// @brief  source file of Query Tree Structure 
/// @author Dohyun Yun
/// @date   2010.06.17 (First Created)
/// 

#include "QueryTree.h"

#include <sstream>

namespace sf1v5 {

    QueryTree::QueryTree() : type_(QueryTree::UNKNOWN)
    {
    } // end - QueryTree()

    QueryTree::QueryTree(QueryType type) : type_(type)
    {
    } // end - QueryTree()

    void QueryTree::insertChild(const boost::shared_ptr<QueryTree>& childQueryTree)
    {
        children_.push_back( childQueryTree );
    } // end - insertChild()

    void QueryTree::print(std::ostream& out, int tabLevel) const
    {
        using namespace std;
        stringstream ss;

        for(int i = 0; i < tabLevel; i++) ss << " ";
        ss << "[";
        switch( type_ )
        {
            case QueryTree::KEYWORD:
                ss << "KEYWORD  :" << keyword_ << "]" << endl;
                break;
            case QueryTree::WILDCARD:
                ss << "WILDCARD :" << keyword_ << "]" << endl;
                break;
            case QueryTree::AND:
                ss << "AND      : ]" << endl;
                break;
            case QueryTree::OR:
                ss << "OR       : ]" << endl;
                break;
            case QueryTree::NOT:
                ss << "NOT      : ]" << endl;
                break;
            case QueryTree::EXACT:
                ss << "EXACT    :" << keyword_ << "]" << endl;
                break;
            case QueryTree::ORDER:
                ss << "ORDER    :" << keyword_ << "]" << endl;
                break;
            case QueryTree::NEARBY:
                ss << "NEARBY   :" << keyword_ << ":distance(" << distance_ << ") ]" << endl;
                break;
            default:
                ss << "UNKNOWN  : ]" << endl;
                break;
        }
        out << ss.str();

        std::list<QueryTreePtr>::const_iterator iter = children_.begin();
        for(; iter != children_.end(); iter++)
            (*iter)->print(out, tabLevel + 3 );
    } // print()
} // end - namespace sf1v5

