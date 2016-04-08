#include "stdafx.h"
#include "CommandLine.h"


namespace plugin_cpp_api
{
   CCommandLine::CCommandLine(int argc, char **argv)
   {
      if (argc != 1)
         throw std::invalid_argument("Invalid arguments number");

      m_yPluginApiAccessorId = argv[0];
   }

   CCommandLine::~CCommandLine()
   {
   }

   std::string CCommandLine::yPluginApiAccessorId() const
   {
      return m_yPluginApiAccessorId;
   }

} // namespace plugin_cpp_api
