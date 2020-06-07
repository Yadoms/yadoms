#include "stdafx.h"
#include "SystemProfilerDevicesLister.h"
#include "SystemProfilerDevice.h"
#include "SystemProfilerCall.h"
#include <shared/Log.h>
#include <regex>

namespace hardware
{
namespace usb
{
std::vector<boost::shared_ptr<IDevice>> CSystemProfilerDevicesLister::listUsbDevices()
{
   try
   {
      std::vector<std::string> args;
      args.push_back("SPUSBDataType");

      CSystemProfilerCall SystemProfilerCall(args);
      const auto lines = SystemProfilerCall.execute(true);

      std::vector<boost::shared_ptr<IDevice>> devicesList;
      int vid = 0;
      int pid = 0;
      std::string name;
       
      for (const auto &line : lines)
      {
         try
         {
            std::smatch matches;
            if (!std::regex_search(line,
                                   matches,
                                   std::regex(std::string("Product ID: (0x?[[:xdigit:]]{4})|Vendor ID: (0x?[[:xdigit:]]{4}) \\s*\\(([^)]*)"))))
               continue;
             
            if(matches[1].matched)
                pid = std::stoi(std::string(matches[1].first, matches[1].second), nullptr, 16);
            if(matches[2].matched)
                vid = std::stoi(std::string(matches[2].first, matches[2].second), nullptr, 16);
            if(matches[2].matched)
                name = matches[3];
            if(pid != 0  && vid != 0 && !name.empty())
            {
                devicesList.emplace_back(boost::make_shared<CSystemProfilerDevice>(vid,
                                                                          pid,
                                                                          name));
                pid = 0;
                vid = 0;;
                name.clear();
            }

         }
         catch (const std::exception &e)
         {
            YADOMS_LOG(warning) << "Unable to access device, " << e.what();
         }
      }
      return devicesList;
   }
   catch (const std::exception &e)
   {
      YADOMS_LOG(warning) << "Unable to list USB devices, " << e.what();
      return std::vector<boost::shared_ptr<IDevice>>();
   }
}
} // namespace usb
} // namespace hardware
