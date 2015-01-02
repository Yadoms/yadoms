#include "stdafx.h"
#include "RaspBerryPiSystemDisksList.h"
#include <shared/Log.h>
#include <shared/exception/Exception.hpp>
#include <boost/regex.hpp> 
#include <boost/lexical_cast.hpp>
#include <fstream>
#include <iostream>
#include <unistd.h>

CRaspBerryPiSystemDisksList::CRaspBerryPiSystemDisksList(void)
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

CRaspBerryPiSystemDisksList::~CRaspBerryPiSystemDisksList()
{}

std::vector<std::string> CRaspBerryPiSystemDisksList::ExecuteCommandAndReturn(const std::string &szCommand)
{
   std::vector<std::string> ret;

   FILE *fp;

   /* Open the command for reading. */

   fp = popen(szCommand.c_str(), "r");

   if (fp != NULL) 
   {
      char path[1035];
      /* Read the output a line at a time - output it. */
      while (fgets(path, sizeof(path)-1, fp) != NULL)
      {
         ret.push_back(path);
      }
      /* close */
      pclose(fp);
   }
   return ret;
}


std::vector<std::string>& CRaspBerryPiSystemDisksList::getList()
{
   return DrivesList;
}


