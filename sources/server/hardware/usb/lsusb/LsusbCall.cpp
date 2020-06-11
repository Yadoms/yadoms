#include "stdafx.h"
#include "LsusbCall.h"
#include <shared/Log.h>
#include <shared/process/Process.h>
#include <shared/process/NativeExecutableCommandLine.h>

namespace hardware
{
namespace usb
{
class CLsusbProcessLogger final : public shared::process::IExternalProcessLogger
{
public:
   explicit CLsusbProcessLogger() = default;
   virtual ~CLsusbProcessLogger() = default;

   // IExternalProcessLogger Implementation
   void init() override {}
   void information(const std::string &line) override { m_stdoutContent.push_back(line); }
   void error(const std::string &line) override { m_stderrContent += line + "\n"; }
   void purgeLogFile() override {}
   // [END] IExternalProcessLogger Implementation

   const std::string &stderrContent() const { return m_stderrContent; }
   const std::vector<std::string> &stdoutContent() const { return m_stdoutContent; }

private:
   std::string m_stderrContent;
   std::vector<std::string> m_stdoutContent;
};

class CLsusbProcessObserver final : public shared::process::IProcessObserver
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

CLsusbCall::CLsusbCall(const std::vector<std::string> &parameters)
    : m_parameters(parameters)
{
}

std::vector<std::string> CLsusbCall::execute(bool ignoreError)
{
   const auto commandLine = boost::make_shared<shared::process::CNativeExecutableCommandLine>("lsusb",
                                                                                              ".",
                                                                                              m_parameters);
   const auto processOut = boost::make_shared<CLsusbProcessLogger>();
   const auto processObserver = boost::make_shared<CLsusbProcessObserver>();
   const shared::process::CProcess process(commandLine,
                                           processObserver,
                                           processOut);

   if (!processObserver->waitFinish(std::chrono::seconds(20)))
      throw std::runtime_error("Unable to list USB devices : timeout executing lsusb");

   if (!processOut->stderrContent().empty() && !ignoreError)
      throw std::runtime_error("Unable to list USB devices : " + processOut->stderrContent());

   return processOut->stdoutContent();
}
} // namespace usb
} // namespace hardware
