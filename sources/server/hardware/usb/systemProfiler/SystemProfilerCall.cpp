#include "stdafx.h"
#include "SystemProfilerCall.h"
#include <shared/Log.h>
#include <shared/process/Process.h>
#include <shared/process/NativeExecutableCommandLine.h>

namespace hardware
{
namespace usb
{
class CSystemProfilerProcessLogger final : public shared::process::IExternalProcessLogger
{
public:
   explicit CSystemProfilerProcessLogger() = default;
   virtual ~CSystemProfilerProcessLogger() = default;

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

class CSystemProfilerProcessObserver final : public shared::process::IProcessObserver
{
public:
   explicit CSystemProfilerProcessObserver() = default;
   virtual ~CSystemProfilerProcessObserver() = default;

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

CSystemProfilerCall::CSystemProfilerCall(const std::vector<std::string> &parameters)
    : m_parameters(parameters)
{
}

std::vector<std::string> CSystemProfilerCall::execute(bool ignoreError)
{
   const auto commandLine = boost::make_shared<shared::process::CNativeExecutableCommandLine>("system_profiler",
                                                                                              ".",
                                                                                              m_parameters);
   const auto processOut = boost::make_shared<CSystemProfilerProcessLogger>();
   const auto processObserver = boost::make_shared<CSystemProfilerProcessObserver>();
   const shared::process::CProcess process(commandLine,
                                           processObserver,
                                           processOut);

   if (!processObserver->waitFinish(std::chrono::seconds(20)))
      throw std::runtime_error("Unable to list USB devices : timeout executing SystemProfiler");

   if (!processOut->stderrContent().empty() && !ignoreError)
      throw std::runtime_error("Unable to list USB devices : " + processOut->stderrContent());

   return processOut->stdoutContent();
}
} // namespace usb
} // namespace hardware
