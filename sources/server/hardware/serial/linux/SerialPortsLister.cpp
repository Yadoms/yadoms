#include "stdafx.h"
#include <shared/Log.h>
#include "SerialPortsLister.h"
#include <termios.h>

namespace hardware
{
   namespace serial
   {
      std::vector<boost::shared_ptr<database::entities::CSerialPort>> CSerialPortsLister::listSerialPorts()
      {
         boost::filesystem::path ttyDir("/sys/class/tty");

         std::vector<boost::shared_ptr<database::entities::CSerialPort>> serialPorts;

         if (boost::filesystem::exists(ttyDir) && boost::filesystem::is_directory(ttyDir))
         {
            boost::filesystem::directory_iterator endIter;
            for (boost::filesystem::directory_iterator dirIter(ttyDir) ; dirIter != endIter ; ++dirIter)
            {
               if (boost::filesystem::is_directory(*dirIter) && boost::filesystem::exists(*dirIter / "device"))
               {
                  std::string friendlyName((*dirIter).path().leaf().string());            // friendlyName is something like "tty0"
                  std::string portName((boost::format("/dev/%1%") % friendlyName).str()); // portName is "/dev/tty0"

                  auto port = boost::make_shared<database::entities::CSerialPort>();
                  port->AdapterKind = database::entities::ESerialPortAdapterKind::kPhysical; //TODO à corriger
                  port->AdapterDescription = friendlyName;
                  port->LastKnownConnectionPath = portName;
						serialPorts.emplace_back(port);
               }
            }
         }

         return serialPorts;
      }
   } // namespace serial
} // namespace hardware
