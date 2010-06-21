///
/// @brief  source file of Query Parser
/// @author Dohyun Yun
/// @date   2010.06.16 (First Created)
/// 

#include "QueryParser.h"

namespace sf1v5 {

    // Declare static variables
    std::string QueryParser::operStr_;
    boost::unordered_map<char, bool> QueryParser::openBracket_;
    boost::unordered_map<char, bool> QueryParser::closeBracket_;

    void QueryParser::initOnlyOnce()
    {
        static boost::once_flag once = BOOST_ONCE_INIT;
        boost::call_once(&initOnlyOnceCore, once);
    } // end - initOnlyOnce()

    void QueryParser::initOnlyOnceCore()
    {
        using namespace std;
        operStr_.assign(" |!(){}[]^\"");

        openBracket_.insert( make_pair('(',true) );
        openBracket_.insert( make_pair('[',true) );
        openBracket_.insert( make_pair('{',true) );
        openBracket_.insert( make_pair('"',true) );

        closeBracket_.insert( make_pair(')',true) );
        closeBracket_.insert( make_pair(']',true) );
        closeBracket_.insert( make_pair('}',true) );
        closeBracket_.insert( make_pair('"',true) );

    } // end - initOnlyOnceCore()

    void QueryParser::normalizeQuery(const std::string& queryString, std::string& normString)
    {
        initOnlyOnce();
        std::string tmpString, tmpNormString;
        std::string::const_iterator iter, iterEnd, prevIter;

        // -----[ Step 1 : Remove continuous space and the space before and after | ]
        iter = queryString.begin();
        iterEnd = queryString.end();
        while( iter != iterEnd )
        {
            if ( *iter == ' ' ) { 
                // (   hello kity   ) -> ( hello kity ) 
                // (  hello     | kity ) -> ( hello| kitty )
                while( ++iter != iterEnd && *iter == ' ' );
                if ( iter != iterEnd && *iter != '|')
                    tmpString.push_back(' ');
            }
            else if ( *iter == '|' ) { 
                // (Hello|  kity) -> (Hello|kity)
                while( ++iter != iterEnd && *iter == ' ' );
                tmpString.push_back('|');
            } // end - else if
            else tmpString.push_back( *iter++ );
        } // end - while

        // -----[ Step 2 : Remove or add space after or behind of specific operator ]
        iter = tmpString.begin();
        iterEnd = tmpString.end();
        while(iter != iterEnd)
        {
            switch(*iter) {
                case '!': case '|': case '(': case '[': case '{': case '}':
                    // ( hello world) -> (hello world)
                    tmpNormString.push_back( *iter++ );
                    if ( iter != iterEnd && *iter == ' ' ) iter++; 
                    break;
                case ')': case ']':
                    // (test keyword)attach -> (test keyword) attach
                    tmpNormString.push_back( *iter++ );
                    if ( iter != iterEnd && *iter != ' ' && *iter != '|')
                        tmpNormString.push_back(' ');
                    break;
                case '^': 
                    // Remove space between ^ and number and add space between number and open bracket.
                    // {Test case}^ 123(case) -> {Test case}^123 (case)
                    tmpNormString.push_back( *iter++ );
                    if ( iter != iterEnd && *iter == ' ' ) iter++; 

                    while( iter != iterEnd && isdigit(*iter) ) // Store digit
                        tmpNormString.push_back( *iter++ );

                    if ( openBracket_[*iter] ) // if first char after digit is open bracket, insert space.
                        tmpNormString.push_back(' ');
                    break;
                case ' ': // (hello world ) -> (hello world)
                    if ( ++iter != iterEnd && !closeBracket_[*iter] )
                        tmpNormString.push_back(' ');
                    break;
                case '"': // Skip all things inside the exact bracket.
                    tmpNormString.push_back('"');
                    while( ++iter != iterEnd && *iter != '"') tmpNormString.push_back( *iter );
                    tmpNormString.push_back( *iter++ ); // insert last "
                    break;
                default: // Store char && add space if openBracket is attached to the back of closeBracket.
                    //prevIter = iter;
                    tmpNormString.push_back( *iter++ );
                    if ( (iter != iterEnd) && (openBracket_[*iter] || *iter == '!') )
                        tmpNormString.push_back(' ');
            } // end - switch()
        } // end - while

        normString.swap( tmpNormString );

    } // end - normalizeQuery()

    bool QueryParser::parseQuery( const std::string& queryString, QueryTreePtr& queryTree )
    {
        std::string normQueryString;
        normalizeQuery(queryString, normQueryString);
        QueryParser qGrammar;
        tree_parse_info<> info = ast_parse(normQueryString.c_str(), qGrammar);
        if ( info.full )
            getQueryTree(info.trees.begin(), queryTree);
        return info.full;
    } // end - parseQuery()

    void QueryParser::getQueryTree(iter_t const& i, QueryTreePtr& queryTree)
    {
        if ( i->value.id() == stringQueryID )
            processKeywordAssignQuery(i, queryTree);
        else if  (i->value.id() == exactQueryID )
            processBracketQuery(i, QueryTree::EXACT, queryTree);
        else if ( i->value.id() == orderedQueryID )
            processBracketQuery(i, QueryTree::ORDER, queryTree);
        else if ( i->value.id() == nearbyQueryID )
            processBracketQuery(i, QueryTree::NEARBY, queryTree);
        else if ( i->value.id() == boolQueryID )
            processBoolQuery(i, queryTree);
        else 
            processChildTree(i, queryTree);
    } // end - getQueryTree()

    void QueryParser::processKeywordAssignQuery( iter_t const& i, QueryTreePtr& queryTree)
    {
        QueryTree::QueryType queryType(QueryTree::KEYWORD);
        std::string tmpString( i->value.begin(), i->value.end() );
        if ( tmpString.find('*') != std::string::npos || tmpString.find('?') != std::string::npos )
            queryType = QueryTree::WILDCARD;
        QueryTreePtr tmpQueryTree(new QueryTree(queryType));
        tmpQueryTree->keyword_.swap( tmpString );
        queryTree.swap( tmpQueryTree );
    } // end - processKeywordAssignQuery()

    void QueryParser::processBracketQuery(iter_t const& i, QueryTree::QueryType queryType, QueryTreePtr& queryTree)
    {
        QueryTreePtr tmpQueryTree(new QueryTree(queryType));

        // Store String value of first child into keyword_
        iter_t childIter = i->children.begin();
        std::string tmpString( childIter->value.begin(), childIter->value.end() );
        tmpQueryTree->keyword_.swap( tmpString );

        if ( queryType == QueryTree::NEARBY )
        {
            // Store distance
            iter_t distIter = i->children.begin()+1;
            std::string distStr( distIter->value.begin(), distIter->value.end() );
            tmpQueryTree->distance_ = atoi( distStr.c_str() );
        }

        queryTree.swap( tmpQueryTree );
    } // end - processBracketQuery()

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
                // Move all children in second node into first ( Insert : first->end )
                (*first)->children_.splice( (*first)->children_.end(), (*second)->children_ );
            else 
                // Move second node into children in first ( Insert : first->end )
                (*first)->children_.push_back( *second );

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
        if ( i->value.id() == notQueryID ) queryType = QueryTree::NOT;
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
