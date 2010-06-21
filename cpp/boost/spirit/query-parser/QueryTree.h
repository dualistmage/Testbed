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

            ///
            /// @brief  a variable which indicates type of current query node.
            ///
            enum QueryType { KEYWORD, WILDCARD, AND, OR, NOT, EXACT, ORDER, NEARBY, UNKNOWN } type_; 

            ///
            /// @brief keyword string of current query node.
            ///
            std::string keyword_;

            ///
            /// @brief this variable is used only for Nearby Type.  { A B }^distance_
            ///
            int distance_;

            ///
            /// @brief a list of chidren which current query node has.
            ///
            std::list<boost::shared_ptr<QueryTree> > children_;

        public:
            QueryTree();
            QueryTree(QueryType type);

            ///
            /// @brief inserts child node inth current node.
            /// @param[childQueryTree] child query tree which is inserted into current node.
            ///
            void insertChild(const boost::shared_ptr<QueryTree>& childQueryTree);

            ///
            /// @brief display content of current tree node from root to leaf.
            /// @param[out] ostream where content of tree is displayed.
            /// @param[tabLevel] This parameter indicates how many tabs are used before displaying current node.
            ///
            void print(std::ostream& out = std::cout, int tabLevel = 0) const;


    }; // end - struct QueryTree

    typedef boost::shared_ptr<QueryTree> QueryTreePtr;
    typedef std::list<QueryTreePtr>::iterator QueryTreeIter;

} // end - namespace sf1v5

#endif // _QUERY_TREE_H_
