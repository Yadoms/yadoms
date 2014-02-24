#include "stdafx.h"

#include "../../shared/exceptions/NotSupported.hpp"
#include "../../shared/Log.h"
#include "Peripherals.h"

namespace shared
{

CPeripherals::CPeripherals()
{
}

CPeripherals::~CPeripherals()
{
}

const boost::shared_ptr<CPeripherals::SerialPortsMap> CPeripherals::getSerialPorts()
{
   boost::filesystem::path ttyDir("/sys/class/tty");

   boost::shared_ptr<SerialPortsMap> serialPorts(new SerialPortsMap);

   if (boost::filesystem::exists(ttyDir) && boost::filesystem::is_directory(ttyDir))
   {
      boost::filesystem::directory_iterator endIter;
      for (boost::filesystem::directory_iterator dirIter(ttyDir) ; dirIter != endIter ; ++dirIter)
      {
         if (boost::filesystem::is_directory(*dirIter) && boost::filesystem::exists(*dirIter / "device"))
         {
            std::string portName((*dirIter).path().leaf().string());
            (*serialPorts)[portName]=portName;
         }
      }
   }

   return serialPorts;
}

} // namespace shared
