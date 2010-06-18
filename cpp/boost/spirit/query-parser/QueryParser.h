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

            static const int complexID          = 1;
            static const int stringQueryID      = 100;
            static const int boolQueryID        = 200;
            static const int andQueryID         = 201;
            static const int orQueryID          = 202;
            static const int notQueryID         = 203;
            static const int exactQueryID       = 300;
            static const int exactStringID      = 401;
            static const int orderedQueryID     = 500;
            static const int nearbyQueryID      = 600;

            template <typename ScannerT> struct definition
            {
                definition(const QueryParser&) // Rule definition area
                {
                    static const std::string operStr(" |!(){}[]^\"");

                    query = root_node_d[+( stringQuery | exactQuery | boolQuery | orderedQuery | nearbyQuery )];

                    exactQuery = root_node_d[ch_p('\"')] >> exactString >> no_node_d[ch_p('\"')];
                    exactString = leaf_node_d[ lexeme_d[+(~ch_p('\"'))] ];

                    stringQuery = leaf_node_d[ lexeme_d[+(~chset_p(operStr.c_str()))] ];

                    orderedQuery = root_node_d[ch_p('[')] >> stringQuery >> no_node_d[ch_p(']')];
                    nearbyQuery = root_node_d[ch_p('{')] >> stringQuery >> no_node_d[ch_p('}')] >> no_node_d[ch_p('^')] >> +digit_p;
                    
                    /*
                    boolQuery = andQuery | orQuery | notQuery;
                    andQuery = query >> +(root_node_d[ch_p(' ')] >> query); 
                    orQuery = query >> +(root_node_d[ch_p('|')] >> query); 
                    notQuery = root_node_d[ch_p('!')] >> query;
                    */
                    

                } // end - definition()

                rule<ScannerT, parser_context<>, parser_tag<complexID> > query;
                rule<ScannerT, parser_context<>, parser_tag<stringQueryID> > stringQuery;
                rule<ScannerT, parser_context<>, parser_tag<boolQueryID> > boolQuery;
                rule<ScannerT, parser_context<>, parser_tag<andQueryID> > andQuery;
                rule<ScannerT, parser_context<>, parser_tag<orQueryID> > orQuery;
                rule<ScannerT, parser_context<>, parser_tag<notQueryID> > notQuery;
                rule<ScannerT, parser_context<>, parser_tag<exactQueryID> > exactQuery;
                rule<ScannerT, parser_context<>, parser_tag<exactStringID> > exactString;
                rule<ScannerT, parser_context<>, parser_tag<orderedQueryID> > orderedQuery;
                rule<ScannerT, parser_context<>, parser_tag<nearbyQueryID> > nearbyQuery;

                rule<ScannerT> notOper;

                rule<ScannerT, parser_context<>, parser_tag<complexID> > const& start() const { return query; } // Return Start rule of rule in 
            }; // end - definition

        private:

            //static boost::unordered_map< int , QueryTree::QueryType > queryTypeMap_; // ID - type enum

        public: // static interface

            //static void initOnlyOnce();
            static bool parseQuery( const std::string& queryString, QueryTreePtr& queryTree );

        private:

            //static void initOnlyOnceCore();
            static void getQueryTree(iter_t const& i, QueryTreePtr& queryTree);
            static void processChildTree(iter_t const& i, QueryTreePtr& queryTree);


    }; // end - struct QueryParser.h

} // end - namespace sf1v5

#endif // _QUERY_PARSER_H_
