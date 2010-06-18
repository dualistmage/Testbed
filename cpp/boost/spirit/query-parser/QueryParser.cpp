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
        std::cout << "//////////////////  i->value : " << std::string(i->value.begin(), i->value.end()) 
            << " (child size : " << i->children.size() << ")" << std::endl;

        if  (i->value.id() == exactStringID ) {
            std::cout << "ExactString : " << std::endl;

            // Create temporary query tree of KEYWORD query type
            QueryTreePtr tmpQueryTree(new QueryTree(QueryTree::KEYWORD));
            tmpQueryTree->keyword_.assign( i->value.begin(), i->value.end() );
            queryTree.swap( tmpQueryTree );
        } // end -exactString()
        else if ( i->value.id() == stringQueryID ) {
            std::cout << "String : " << std::endl;
            std::string tmpString( i->value.begin(), i->value.end() );

            QueryTreePtr tmpQueryTree;

            // Find if wildcard char is in the string.
            if ( tmpString.find('*') != std::string::npos || tmpString.find('?') != std::string::npos ) 
                tmpQueryTree.reset(new QueryTree(QueryTree::WILDCARD));
            else
                tmpQueryTree.reset(new QueryTree(QueryTree::KEYWORD));

            tmpQueryTree->keyword_.swap( tmpString );
            queryTree.swap( tmpQueryTree );
        } // end - stringQuery
        else if ( i->value.id() == orderedQueryID ) {
            std::cout << "Ordered : " << std::endl;

            QueryTreePtr tmpQueryTree(new QueryTree(QueryTree::ORDER));
            tmpQueryTree->keyword_.assign( i->children.begin()->value.begin(), i->children.begin()->value.end() );
            queryTree.swap( tmpQueryTree );
        } // end - orderedQueryID
        else if ( i->value.id() == nearbyQueryID ) {
            std::cout << "NearBy : " << std::endl;

            QueryTreePtr tmpQueryTree(new QueryTree(QueryTree::NEARBY));
            tmpQueryTree->keyword_.assign( i->children.begin()->value.begin(), i->children.begin()->value.end() );

            std::string distance;
            iter_t iter = i->children.begin()+1;
            for(; iter != i->children.end(); iter++)
                distance += *(iter->value.begin());
            tmpQueryTree->distance_ = atoi(distance.c_str());

            queryTree.swap( tmpQueryTree );
        } // end - orderedQueryID
        else {
            processChildTree(i, queryTree);
        }



    } // end - getQueryTree()

    void QueryParser::processChildTree(iter_t const& i, QueryTreePtr& queryTree)
    {
        QueryTree::QueryType queryType;
        if ( i->value.id() == complexID ) queryType = QueryTree::COMPLEX;
        else if ( i->value.id() == andQueryID ) queryType = QueryTree::AND;
        else if ( i->value.id() == orQueryID ) queryType = QueryTree::OR;
        else if ( i->value.id() == notQueryID ) queryType = QueryTree::NOT;
        else if ( i->value.id() == exactQueryID ) queryType = QueryTree::EXACT;
        else if ( i->value.id() == nearbyQueryID ) queryType = QueryTree::NEARBY;
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
