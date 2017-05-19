#include "stdafx.h"
#include "DisksList.h"
#include "LinuxHelpers.h"
#include <shared/exception/Exception.hpp>
#include <boost/regex.hpp> 
#include <boost/lexical_cast.hpp>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <shared/Log.h>

CDisksList::CDisksList()
{
   std::vector<std::string> _rlines = ExecuteCommandAndReturn("df");
   std::vector<std::string>::const_iterator iteratorCommandDF;

   for (iteratorCommandDF = _rlines.begin(); iteratorCommandDF != _rlines.end(); ++iteratorCommandDF)
   {
      boost::regex reg("(.(dev)[^\\s]+)");
      std::string dname;
      boost::smatch match;

      if ( boost::regex_search( *iteratorCommandDF, match, reg ) )
      {
         dname = boost::lexical_cast<std::string>(match[1]);
      }
      if (!dname.empty())
      {
         DrivesList.push_back( dname.c_str() );
         YADOMS_LOG(information) << "found:" << dname;
      }
   }
}

CDisksList::~CDisksList()
{
}

const std::vector<std::string>& CDisksList::getList() const
{
   return DrivesList;
}

