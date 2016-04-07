#include "stdafx.h"
#include "PluginContext.h"
#include "ApiImplementation.h"
#include "CommandLine.h"


namespace plugin_cpp_api
{
   CPluginContext::CPluginContext(int argc, char **argv)
   {
      auto commandLine(argc, argv);
      m_api = boost::make_shared<CApiImplementation>(commandLine.yPluginApiAccessorId());
   }

   CPluginContext::~CPluginContext()
   {
   }

   boost::shared_ptr<shared::plugin::yPluginApi::IYPluginApi> CPluginContext::api() const
   {
      return m_api;
   }

} // namespace plugin_cpp_api
