///
/// @brief  header file of Query Tree Structure 
/// @author Dohyun Yun
/// @date   2010.06.16 (First Created)
/// 

#ifndef _QUERY_TREE_H_
#define _QUERY_TREE_H_

#include <boost/shared_ptr.hpp>
#include <iostream>
#include <list>

namespace sf1v5 {

    class QueryTree
    {
        public:
            enum QueryType { KEYWORD, WILDCARD, AND, OR, NOT, EXACT, ORDER, NEARBY, UNKNOWN } type_; 
            std::list<boost::shared_ptr<QueryTree> > children_;

            // For KEYWORD and WILDCARD Type
            std::string keyword_;

            // For only Nearby Type  { A B }^distance_
            int distance_;

        public:
            QueryTree();
            QueryTree(QueryType type);

            void insertChild(const boost::shared_ptr<QueryTree>& childQueryTree);

            void print(std::ostream& out = std::cout, int tabLevel = 0) const;


    }; // end - struct QueryTree

    typedef boost::shared_ptr<QueryTree> QueryTreePtr;
    typedef std::list<QueryTreePtr>::iterator QueryTreeIter;

} // end - namespace sf1v5

#endif // _QUERY_TREE_H_
