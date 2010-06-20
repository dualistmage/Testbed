///
/// @brief  source file of Query Parser
/// @author Dohyun Yun
/// @date   2010.06.16 (First Created)
/// 

#include "QueryParser.h"

namespace sf1v5 {

    /*
    boost::unordered_map< int , QueryTree::QueryType > queryTypeMap_;

    void QueryParser::initOnlyOnce()
    {
        static boost::once_flag once = BOOST_ONCE_INIT;
        boost::call_once(&initOnlyOnceCore, once);
    } // end - initOnlyOnce()

    void QueryParser::initOnlyOnceCore()
    {
        using namespace std;
        queryTypeMap_.insert( make_pair( complexID      , QueryTree::COMPLEX ) );
        queryTypeMap_.insert( make_pair( andQueryID     , QueryTree::AND     ) );
        queryTypeMap_.insert( make_pair( orQueryID      , QueryTree::OR      ) );
        queryTypeMap_.insert( make_pair( notQueryID     , QueryTree::NOT     ) );
        queryTypeMap_.insert( make_pair( exactQueryID   , QueryTree::EXACT   ) );
        queryTypeMap_.insert( make_pair( orderedQueryID , QueryTree::ORDER   ) );
        queryTypeMap_.insert( make_pair( nearbyQueryID  , QueryTree::NEARBY  ) );
    } // end - initOnlyOnceCore()
    */

    bool QueryParser::parseQuery( const std::string& queryString, QueryTreePtr& queryTree )
    {
        QueryParser qGrammar;
        tree_parse_info<> info = ast_parse(queryString.c_str(), qGrammar);
        if ( info.full )
            getQueryTree(info.trees.begin(), queryTree);

        return info.full;
    } // end - parseQuery()

    void QueryParser::getQueryTree(iter_t const& i, QueryTreePtr& queryTree)
    {
        std::cout << "------------------------[ i->value : " 
            << std::string(i->value.begin(), i->value.end()) 
            << " (child size : " << i->children.size() << ")" << std::endl;

        if ( i->value.id() == stringQueryID )
            processKeywordAssignQuery(i, QueryTree::KEYWORD, queryTree);
        else if  (i->value.id() == exactQueryID )
            processKeywordAssignQuery(i, QueryTree::EXACT, queryTree);
        else if ( i->value.id() == orderedQueryID ) {
            processKeywordAssignQuery(i, QueryTree::ORDER, queryTree);
        } // end - orderedQueryID
        else if ( i->value.id() == nearbyQueryID ) {
            QueryTreePtr tmpQueryTree;
            processKeywordAssignQuery(i, QueryTree::NEARBY, tmpQueryTree);

            // Store distance
            iter_t distIter = i->children.begin()+1;
            std::string distStr( distIter->value.begin(), distIter->value.end() );
            tmpQueryTree->distance_ = atoi( distStr.c_str() );

            queryTree.swap( tmpQueryTree );
        } // end - orderedQueryID
        else if ( i->value.id() == boolQueryID )
            processBoolQuery(i, queryTree);
        else 
            processChildTree(i, queryTree);
    } // end - getQueryTree()

    void QueryParser::processKeywordAssignQuery( iter_t const& i, 
            QueryTree::QueryType queryType, QueryTreePtr& queryTree)
    {
            std::string tmpString( i->value.begin(), i->value.end() );
            if ( queryType == QueryTree::KEYWORD 
                    && (tmpString.find('*') != std::string::npos || tmpString.find('?') != std::string::npos ) )
                queryType = QueryTree::WILDCARD;
            QueryTreePtr tmpQueryTree(new QueryTree(queryType));
            tmpQueryTree->keyword_.swap( tmpString );
            queryTree.swap( tmpQueryTree );
    } // end - processKeywordAssignQuery()

    void QueryParser::processBoolQuery(iter_t const& i, QueryTreePtr& queryTree)
    {
        QueryTree::QueryType queryType;
        switch( *(i->value.begin() ) ) {
            case ' ': queryType = QueryTree::AND; break;
            case '|': queryType = QueryTree::OR;  break;
            default: queryType = QueryTree::UNKNOWN; 
        } // end - switch
        QueryTreePtr tmpQueryTree( new QueryTree(queryType) );

        // Retrieve child query tree and insert it to the children_ list of tmpQueryTree.
        iter_t iter = i->children.begin();
        for(; iter != i->children.end(); iter++)
        {
            QueryTreePtr tmpChildQueryTree;
            getQueryTree( iter, tmpChildQueryTree);
            tmpQueryTree->insertChild( tmpChildQueryTree );
        }

        // Merge unnecessarily divided bool-trees
        //   - Children are always two : Binary Tree
        QueryTreeIter first = tmpQueryTree->children_.begin();
        QueryTreeIter second = first; second++;
        if ( (*first)->type_ == queryType )
        { 
            // use first child tree as a root tree.
            if ( (*second)->type_ == queryType )
            {
                // Move all children in second node into first ( Insert : first->end )
                (*first)->children_.splice( (*first)->children_.end(), (*second)->children_ );
            } // end - if
            else
            {
                // Move second node into children in first ( Insert : first->end )
                (*first)->children_.push_back( *second );
            } // end - else

            tmpQueryTree.swap( *first );
        }
        else if ((*second)->type_ == queryType )
        {
            // Move first node into children in second. ( Insert : second->begin )
            (*second)->children_.push_front( *first );
            tmpQueryTree.swap( *second );
        }
        
        tmpQueryTree.swap( queryTree );
    } // end - processBoolQuery()

    void QueryParser::processChildTree(iter_t const& i, QueryTreePtr& queryTree)
    {
        QueryTree::QueryType queryType;
        if ( i->value.id() == rootQueryID ) queryType = QueryTree::COMPLEX;
        else if ( i->value.id() == notQueryID ) queryType = QueryTree::NOT;
        else queryType = QueryTree::UNKNOWN;

        QueryTreePtr tmpQueryTree( new QueryTree(queryType) );

        // Retrieve child query tree and insert it to the children_ list of tmpQueryTree.
        iter_t iter = i->children.begin();
        for(; iter != i->children.end(); iter++)
        {
            QueryTreePtr tmpChildQueryTree;
            getQueryTree( iter, tmpChildQueryTree);
            tmpQueryTree->insertChild( tmpChildQueryTree );
        }
        tmpQueryTree.swap( queryTree );
    } // end - processChildTree



} // end - namespace sf1v5
