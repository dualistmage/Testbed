///
/// @brief  header file of Query Parser
/// @author Dohyun Yun
/// @date   2010.06.16 (First Created)
/// 

#ifndef _QUERY_PARSER_H_
#define _QUERY_PARSER_H_

#define BOOST_SPIRIT_DEBUG 

#include "QueryTree.h"

#include <boost/spirit/include/classic_core.hpp>
#include <boost/spirit/include/classic_ast.hpp>
#include <boost/spirit/include/classic_chset.hpp>
#include <boost/unordered_map.hpp>
#include <boost/thread/once.hpp>
#include <boost/shared_ptr.hpp>

#include <iostream>

namespace sf1v5 {

    using namespace BOOST_SPIRIT_CLASSIC_NS;

    class QueryParser : public grammar<QueryParser>
    {
        public:
            typedef char const* iterator_t;
            typedef tree_match<iterator_t> parse_tree_match_t;
            typedef parse_tree_match_t::tree_iterator iter_t;

            static const int rootQueryID        = 100;
            static const int stringQueryID      = 200;
            static const int boolQueryID        = 300;
            static const int boolRootQueryID    = 301;
            static const int andQueryID         = 302;
            static const int orQueryID          = 303;
            static const int notQueryID         = 304;
            static const int exactQueryID       = 400;
            static const int exactStringID      = 401;
            static const int orderedQueryID     = 500;
            static const int orderedStringID    = 501;
            static const int nearbyQueryID      = 600;
            static const int nearbyStringID     = 601;
            static const int priorQueryID       = 700;

            template <typename ScannerT> struct definition
            {
                definition(const QueryParser&) // Rule definition area
                {
                    static const std::string operStr(" |!(){}[]^\"");

                    rootQuery = root_node_d[eps_p] >> +boolQuery;

                    boolQuery = (stringQuery | notQuery | priorQuery | exactQuery | orderedQuery | nearbyQuery) 
                        >> !( (root_node_d[ch_p(' ')] >> boolQuery) | (root_node_d[ch_p('|')] >> boolQuery) );
                    notQuery = root_node_d[ch_p('!')] >> (stringQuery | priorQuery);

                    stringQuery = leaf_node_d[ lexeme_d[+(~chset_p(operStr.c_str()))] ];

                    exactQuery = root_node_d[ch_p('\"')] >> exactString >> no_node_d[ch_p('\"')];
                    exactString = leaf_node_d[ lexeme_d[+(~ch_p('\"'))] ];

                    orderedQuery = root_node_d[ch_p('[')] >> orderedString >> no_node_d[ch_p(']')];
                    orderedString = leaf_node_d[ lexeme_d[+(~ch_p(']'))] ];

                    nearbyQuery = root_node_d[ch_p('{')] >> nearbyString >> no_node_d[ch_p('}')] 
                        >> no_node_d[ch_p('^')] >> uint_p;
                    nearbyString = leaf_node_d[ lexeme_d[+(~ch_p('}'))] ];

                    priorQuery = inner_node_d[ch_p('(') >> boolQuery >> ch_p(')')];

                } // end - definition()

                rule<ScannerT, parser_context<>, parser_tag<rootQueryID> > rootQuery;
                rule<ScannerT, parser_context<>, parser_tag<stringQueryID> > stringQuery;
                rule<ScannerT, parser_context<>, parser_tag<boolQueryID> > boolQuery;
                rule<ScannerT, parser_context<>, parser_tag<andQueryID> > andQuery;
                rule<ScannerT, parser_context<>, parser_tag<orQueryID> > orQuery;
                rule<ScannerT, parser_context<>, parser_tag<notQueryID> > notQuery;
                rule<ScannerT, parser_context<>, parser_tag<exactQueryID> > exactQuery;
                rule<ScannerT, parser_context<>, parser_tag<exactStringID> > exactString;
                rule<ScannerT, parser_context<>, parser_tag<orderedQueryID> > orderedQuery;
                rule<ScannerT, parser_context<>, parser_tag<orderedStringID> > orderedString;
                rule<ScannerT, parser_context<>, parser_tag<nearbyQueryID> > nearbyQuery;
                rule<ScannerT, parser_context<>, parser_tag<nearbyStringID> > nearbyString;
                rule<ScannerT, parser_context<>, parser_tag<priorQueryID> > priorQuery;

                rule<ScannerT> notOper;

                rule<ScannerT, parser_context<>, parser_tag<rootQueryID> > const& start() const { return rootQuery; } // Return Start rule of rule in 
            }; // end - definition

        public: // static interface

            //static void initOnlyOnce();
            static bool parseQuery( const std::string& queryString, QueryTreePtr& queryTree );

        private:

            //static void initOnlyOnceCore();
            static void getQueryTree(iter_t const& i, QueryTreePtr& queryTree);
            static void processKeywordAssignQuery( iter_t const& i, QueryTreePtr& queryTree);
            static void processBracketQuery( iter_t const& i, QueryTree::QueryType queryType, QueryTreePtr& queryTree);
            static void processBoolQuery(iter_t const& i, QueryTreePtr& queryTree);
            static void processChildTree(iter_t const& i, QueryTreePtr& queryTree);


    }; // end - struct QueryParser.h

} // end - namespace sf1v5

#endif // _QUERY_PARSER_H_
