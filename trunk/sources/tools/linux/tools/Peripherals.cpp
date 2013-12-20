#include "stdafx.h"
#if defined(linux) || defined(__linux) || defined(__linux__) || defined(__GNU__) || defined(__GLIBC__) 

#include "tools/Exceptions/NotImplementedException.hpp"
#include "tools/Log.h"
#include "Peripherals.h"


CPeripherals::CPeripherals()
{
}

CPeripherals::~CPeripherals()
{
}

const boost::shared_ptr<std::vector<std::string> > CPeripherals::getSerialPorts()
{
   // Voir http://stackoverflow.com/questions/15342804/c-linux-detect-all-serial-ports
   throw CNotImplementedException();


   // TODO : rien de testé, ni compilé, mais ça doit ressembler à ça :
   //boost::filesystem::path ttyDir("/sys/class/tty");
   //boost::filesystem::directory_iterator endIter;

   //boost::shared_ptr<std::vector<std::string> > serialPorts(new std::vector<std::string>);

   //if (boost::filesystem::exists(ttyDir) && boost::filesystem::is_directory(ttyDir))
   //{
   //   for (boost::filesystem::directory_iterator dirIter(ttyDir) ; dirIter != endIter ; ++dirIter)
   //   {
   //      if (boost::filesystem::is_directory(*dirIter) && boost::filesystem::exists(*dirIter / "device"))
   //      {
   //         serialPorts->push_back(*dirIter);
   //      }
   //   }
   //}
}

const boost::shared_ptr<std::vector<std::string> > CPeripherals::getUnusedSerialPorts()
{
   //TODO Voir http://stackoverflow.com/questions/15342804/c-linux-detect-all-serial-ports
   throw CNotImplementedException();
}

const boost::shared_ptr<std::vector<std::string> > CPeripherals::getUsedSerialPorts()
{
   //TODO Voir http://stackoverflow.com/questions/15342804/c-linux-detect-all-serial-ports
   throw CNotImplementedException();
}

#endif