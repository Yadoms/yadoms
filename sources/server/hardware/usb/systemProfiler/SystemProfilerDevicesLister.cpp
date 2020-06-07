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
      const auto lines = SystemProfilerCall.execute();

      std::vector<boost::shared_ptr<IDevice>> devicesList;
      for (const auto &line : lines)
      {
         try
         {
            std::smatch matches;
            if (!std::regex_search(line,
                                   matches,
                                   std::regex(std::string("^Bus.*: ID ([[:xdigit:]]{4}):([[:xdigit:]]{4}) (.*)$"))) ||
                matches.size() != 4)
               continue;

            const auto vid = std::stoi(std::string(matches[1].first, matches[1].second), nullptr, 16);
            const auto pid = std::stoi(std::string(matches[2].first, matches[2].second), nullptr, 16);
            const auto name = matches[3];

            devicesList.emplace_back(boost::make_shared<CSystemProfilerDevice>(vid,
                                                                      pid,
                                                                      name));
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
