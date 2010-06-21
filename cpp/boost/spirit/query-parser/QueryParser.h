///
/// @brief  header file of Query Parser
/// @author Dohyun Yun
/// @date   2010.06.16 (First Created)
/// 

#ifndef _QUERY_PARSER_H_
#define _QUERY_PARSER_H_

#define BOOST_SPIRIT_THREADSAFE
//#define BOOST_SPIRIT_DEBUG 

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

            static const int stringQueryID      = 100;
            static const int boolQueryID        = 200;
            static const int notQueryID         = 201;
            static const int exactQueryID       = 300;
            static const int orderedQueryID     = 400;
            static const int nearbyQueryID      = 500;

            template <typename ScannerT> struct definition
            {
                definition(const QueryParser&) // Rule definition area
                {

                    rootQuery = root_node_d[eps_p] >> +boolQuery;

                    boolQuery = (stringQuery | notQuery | priorQuery | exactQuery | orderedQuery | nearbyQuery) 
                        >> !( (root_node_d[ch_p(' ')] >> boolQuery) | (root_node_d[ch_p('|')] >> boolQuery) );
                    notQuery = root_node_d[ch_p('!')] >> (stringQuery | priorQuery);

                    stringQuery = leaf_node_d[ lexeme_d[+(~chset_p(operStr_.c_str()))] ];

                    exactQuery = root_node_d[ch_p('\"')] >> exactString >> no_node_d[ch_p('\"')];
                    exactString = leaf_node_d[ lexeme_d[+(~ch_p('\"'))] ];

                    orderedQuery = root_node_d[ch_p('[')] >> orderedString >> no_node_d[ch_p(']')];
                    orderedString = leaf_node_d[ lexeme_d[+(~ch_p(']'))] ];

                    nearbyQuery = root_node_d[ch_p('{')] >> nearbyString >> no_node_d[ch_p('}')] 
                        >> no_node_d[ch_p('^')] >> uint_p;
                    nearbyString = leaf_node_d[ lexeme_d[+(~ch_p('}'))] ];

                    priorQuery = inner_node_d[ch_p('(') >> boolQuery >> ch_p(')')];

                } // end - definition()

                rule<ScannerT> rootQuery, priorQuery, exactString, orderedString, nearbyString;
                rule<ScannerT, parser_context<>, parser_tag<stringQueryID> > stringQuery;
                rule<ScannerT, parser_context<>, parser_tag<exactQueryID> > exactQuery;
                rule<ScannerT, parser_context<>, parser_tag<orderedQueryID> > orderedQuery;
                rule<ScannerT, parser_context<>, parser_tag<nearbyQueryID> > nearbyQuery;
                rule<ScannerT, parser_context<>, parser_tag<boolQueryID> > boolQuery;
                rule<ScannerT, parser_context<>, parser_tag<notQueryID> > notQuery;
                rule<ScannerT> const& start() const { return rootQuery; } // Return Start rule of rule in 
            }; // end - definition

        private: // private member variables
            static std::string operStr_;

            static boost::unordered_map<char, bool> openBracket_;  // <bracket char , if close bracket>
            static boost::unordered_map<char, bool> closeBracket_;  // <bracket char , if close bracket>

        public: // static interface

            static void initOnlyOnce();

            ///
            /// @brief normalizes query string before parsing. It removes or adds space.
            /// @details
            ///     - Step 1 : Remove continuous space and the space before and after |.
            ///         - (  hello   |    world  ) -> (hello|world)
            ///     - Step 2 : Remove or add space after or behind of specific operator.
            ///         - ( Hello World) -> (Hello World)
            ///         - {test case } ^ 12(month case) -> {test case}^12 (month case)
            ///         - (bracket close)(open space) -> (bracket close) (open space)
            /// @param[queryString] source query string.
            /// @param[normString] normalized query string.
            /// @return true if success or false.
            ///
            static void normalizeQuery(const std::string& queryString, std::string& normString);

            static bool parseQuery( const std::string& queryString, QueryTreePtr& queryTree );

        private:

            static void initOnlyOnceCore();
            static void getQueryTree(iter_t const& i, QueryTreePtr& queryTree);
            static void processKeywordAssignQuery( iter_t const& i, QueryTreePtr& queryTree);
            static void processBracketQuery( iter_t const& i, QueryTree::QueryType queryType, QueryTreePtr& queryTree);
            static void processBoolQuery(iter_t const& i, QueryTreePtr& queryTree);
            static void processChildTree(iter_t const& i, QueryTreePtr& queryTree);


    }; // end - struct QueryParser.h

} // end - namespace sf1v5

#endif // _QUERY_PARSER_H_
