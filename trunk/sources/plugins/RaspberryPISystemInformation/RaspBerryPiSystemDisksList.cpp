#include "stdafx.h"
#include "RaspBerryPiSystemDisksList.h"
#include <shared/Log.h>
#include <shared/exception/Exception.hpp>

#include <fstream>
#include <iostream>
#include <unistd.h>

CRaspBerryPiSystemDisksList::CRaspBerryPiSystemDisksList(void)
{
   std::vector<std::string> _rlines=ExecuteCommandAndReturn("df");
   std::vector<std::string>::const_iterator iteratorCommandDF;

   for (iteratorCommandDF=_rlines.begin(); iteratorCommandDF!=_rlines.end(); ++iteratorCommandDF)
   {
      char dname[200];
      char suse[30];
      char smountpoint[300];
      long numblock, usedblocks, availblocks;
      int ret=sscanf((*iteratorCommandDF).c_str(), "%199s\t%20ld\t%20ld\t%20ld\t%29s\t%299s\n", dname, &numblock, &usedblocks, &availblocks, suse, smountpoint);
      if (ret==6) // TODO : Comprendre pourquoi 6
      {
         if (strstr(dname,"/dev")!=NULL)
         {
            DrivesList.push_back( dname );
            YADOMS_LOG(debug) << "found:" << dname;
         }
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


