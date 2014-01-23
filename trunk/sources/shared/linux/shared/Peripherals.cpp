#include "stdafx.h"

#include "../../shared/Exceptions/NotSupportedException.hpp"
#include "../../shared/Log.h"
#include "Peripherals.h"


CPeripherals::CPeripherals()
{
}

CPeripherals::~CPeripherals()
{
}

const boost::shared_ptr<std::vector<std::string> > CPeripherals::getSerialPorts()
{
   boost::filesystem::path ttyDir("/sys/class/tty");

   boost::shared_ptr<std::vector<std::string> > serialPorts(new std::vector<std::string>);

   if (boost::filesystem::exists(ttyDir) && boost::filesystem::is_directory(ttyDir))
   {
      boost::filesystem::directory_iterator endIter;
      for (boost::filesystem::directory_iterator dirIter(ttyDir) ; dirIter != endIter ; ++dirIter)
      {
         if (boost::filesystem::is_directory(*dirIter) && boost::filesystem::exists(*dirIter / "device"))
         {
            serialPorts->push_back((*dirIter).path().leaf().string());
         }
      }
   }

   return serialPorts;
}

const boost::shared_ptr<std::vector<std::string> > CPeripherals::getUnusedSerialPorts()
{
   // On Linux, no easy mean to know if a serial port is already in use
   NOT_SUPPORTED;
}

const boost::shared_ptr<std::vector<std::string> > CPeripherals::getUsedSerialPorts()
{
   // On Linux, no easy mean to know if a serial port is already in use
   NOT_SUPPORTED;
}
