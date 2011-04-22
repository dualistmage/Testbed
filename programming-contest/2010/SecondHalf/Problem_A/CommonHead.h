///
/// @file   CommonHead.h
/// @author Dohyun Yun
/// @date   2010.10.02
///

#ifndef _COMMON_HEAD_H_
#define _COMMON_HEAD_H_

#include <iostream>
#include <fstream>
#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>
#include <boost/tokenizer.hpp>

typedef unsigned int ID_T;
typedef boost::unordered_map<std::string, ID_T> IDMAP_T;
typedef boost::char_separator<char> SEP_T;
typedef boost::tokenizer<SEP_T > TOK_T;


#endif // _COMMON_HEAD_H_
