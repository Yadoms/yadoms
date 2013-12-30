#pragma once

typedef boost::property_tree::ptree CJson;

class CSJonParser
{
public:
   static std::vector<std::string> ParseUrl(const std::string & url)
   {
      std::vector<std::string> strs;
      //split on slash or anti slash
      boost::split(strs, url, boost::is_any_of("/\\"), boost::algorithm::token_compress_on);
      //remove empty strings
      strs.erase( std::remove_if( strs.begin(), strs.end(), boost::bind( &std::string::empty, _1 ) ), strs.end() );
      return strs;
   }
};