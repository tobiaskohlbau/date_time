
#ifndef _GDTL_TIME_PARSING_HPP___
#define _GDTL_TIME_PARSING_HPP___
/* Copyright (c) 2002 CrystalClear Software, Inc.
 * Disclaimer & Full Copyright at end of file
 * Author: Jeff Garland
 */

#include "boost/tokenizer.hpp"
#include "boost/lexical_cast.hpp"
#include "boost/gdtl/date_parsing.hpp"
#include "boost/cstdint.hpp"


namespace boost {
namespace gdtl {


  template<class time_duration>
  inline
  time_duration
  parse_delimited_time_duration(const std::string& s)
  {
    unsigned short hour=0, min=0, sec =0;
    boost::int64_t fs=0;
    int pos = 0;
    boost::tokenizer<boost::char_delimiters_separator<char> > tok(s);
    for(boost::tokenizer<>::iterator beg=tok.begin(); beg!=tok.end();++beg){
      switch(pos) {
      case 0: {
	hour = boost::lexical_cast<unsigned short>(*beg);
	break;
      }
      case 1: {
	min = boost::lexical_cast<unsigned short>(*beg);
	break;
      }
      case 2: {
	sec = boost::lexical_cast<unsigned short>(*beg);
	break;
      };
      case 3: {
	//Works around a bug in MSVC 6 library that does not support
	//operator>> thus meaning lexical_cast will fail to compile.
#if (defined(BOOST_MSVC) && (_MSC_VER <= 1200))  // 1200 == VC++ 6.0
	fs = _atoi64(beg->c_str());
#else
	fs = boost::lexical_cast<boost::int64_t>(*beg);
#endif
	break;
      }
      }//switch
      pos++;
    }
    return time_duration(hour, min, sec, fs);
  }

  //TODO this could use some error checking!
  inline
  bool 
  split(const std::string& s,
	     char sep,
	     std::string& first,
	     std::string& second)
  {
    int sep_pos = s.find(sep);
    first = s.substr(0,sep_pos);
    second = s.substr(sep_pos+1);
    return true;
  }

  template<class time_type>
  inline
  time_type
  parse_delimited_time(const std::string& s, char sep)
  {
    typedef typename time_type::time_duration_type time_duration;
    typedef typename time_type::date_type date_type;

    //split date/time on a unique delimiter char such as ' ' or 'T'
    std::string date_string, tod_string;
    split(s, sep, date_string, tod_string);
    //call parse_date with first string
    date_type d = parse_date<date_type>(date_string);
    //call parse_time_duration with remaining string
    time_duration td = parse_delimited_time_duration<time_duration>(tod_string);
    //construct a time
    return time_type(d, td);

  }


} }//namespace gdtl



/* Copyright (c) 2002
 * CrystalClear Software, Inc.
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  CrystalClear Software makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 */

#endif
