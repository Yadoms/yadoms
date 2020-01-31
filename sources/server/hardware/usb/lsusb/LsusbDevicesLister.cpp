#include "stdafx.h"
#include "LsusbDevicesLister.h"
#include "LsusbDevice.h"
#include <shared/Log.h>
#include <shared/process/Process.h>
#include <shared/process/NativeExecutableCommandLine.h>
#include <shared/exception/NotImplemented.hpp>
#include <chrono>
#include <regex>

namespace hardware
{
namespace usb
{
class CLsusbProcessLogger : public shared::process::IExternalProcessLogger // TODO déplacer ?
{
public:
   explicit CLsusbProcessLogger() = default;
   virtual ~CLsusbProcessLogger() = default;

   // IExternalProcessLogger Implementation
   void init() override {}
   void information(const std::string &line) override { m_stdoutContent.push_back(line); }
   void error(const std::string &line) override { m_stderrContent += line + "\n"; }
   void purgeLogFile() override
   {
      throw shared::exception::CNotImplemented("CLsusbProcessLogger::purgeLogFile");
   }
   // [END] IExternalProcessLogger Implementation

   const std::string &stderrContent() const { return m_stderrContent; }
   const std::vector<std::string> &stdoutContent() const { return m_stdoutContent; }

private:
   std::string m_stderrContent;
   std::vector<std::string> m_stdoutContent;
};

class CLsusbProcessObserver : public shared::process::IProcessObserver //TODO déplacer ?
{
public:
   explicit CLsusbProcessObserver() = default;
   virtual ~CLsusbProcessObserver() = default;

   template <typename _Rep, typename _Period>
   bool waitFinish(const std::chrono::duration<_Rep, _Period> &timeout)
   {
      std::unique_lock<std::mutex> lock(m_mutex);
      return m_condition.wait_for(lock,
                                  timeout) == std::cv_status::no_timeout;
   }

protected:
   // IProcessObserver Implementation
   void onStart() override
   {
   }
   void onFinish(int returnCode,
                 const std::string &error) override
   {
      std::unique_lock<std::mutex> lock(m_mutex);
      m_condition.notify_one();
   }
   // [END] IProcessObserver Implementation

private:
   std::mutex m_mutex;
   std::condition_variable m_condition;
};

std::vector<boost::shared_ptr<IDevice>> CLsusbDevicesLister::listUsbDevices()
{
   const auto commandLine = boost::make_shared<shared::process::CNativeExecutableCommandLine>("lsusb",
                                                                                              ".",
                                                                                              std::vector<std::string>());
   const auto processOut = boost::make_shared<CLsusbProcessLogger>();
   const auto processObserver = boost::make_shared<CLsusbProcessObserver>();
   const shared::process::CProcess process(commandLine,
                                           processObserver,
                                           processOut);

   if (!processObserver->waitFinish(std::chrono::seconds(20)))
   {
      YADOMS_LOG(warning) << "Unable to list USB devices : timeout executing lsusb";
      return std::vector<boost::shared_ptr<IDevice>>();
   }

   if (!processOut->stderrContent().empty())
   {
      YADOMS_LOG(warning) << "Unable to list USB devices : " << processOut->stderrContent();
      return std::vector<boost::shared_ptr<IDevice>>();
   }

   std::vector<boost::shared_ptr<IDevice>> devicesList;
   for (const auto &line : processOut->stdoutContent())
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

         devicesList.emplace_back(boost::make_shared<CLsusbDevice>(vid,
                                                                   pid,
                                                                   name));
      }
      catch (const std::exception &e)
      {
         YADOMS_LOG(warning) << "Unable to access device " << e.what();
      }
   }
   return devicesList;
}
} // namespace usb
} // namespace hardware
