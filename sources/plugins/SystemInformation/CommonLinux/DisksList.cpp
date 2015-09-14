#include "stdafx.h"
#include "DisksList.h"
#include "LinuxHelpers.h"
#include <shared/Log.h>
#include <shared/exception/Exception.hpp>
#include <boost/regex.hpp> 
#include <boost/lexical_cast.hpp>
#include <fstream>
#include <iostream>
#include <unistd.h>

CDisksList::CDisksList(void)
{
   std::vector<std::string> _rlines=ExecuteCommandAndReturn("df");
   std::vector<std::string>::const_iterator iteratorCommandDF;

   for (iteratorCommandDF=_rlines.begin(); iteratorCommandDF!=_rlines.end(); ++iteratorCommandDF)
   {
      boost::regex reg("(.(dev)[^\\s]+)");
      std::string dname;
      boost::smatch match;

      if ( boost::regex_search( *iteratorCommandDF, match, reg ) )
      {
        dname       = boost::lexical_cast<std::string>(match[1]);
      }
      if (!dname.empty())
      {
         DrivesList.push_back( dname.c_str() );
         YADOMS_LOG(debug) << "found:" << dname;
      }
   }
}

CDisksList::~CDisksList()
{}


std::vector<std::string>& CDisksList::getList()
{
   return DrivesList;
}


