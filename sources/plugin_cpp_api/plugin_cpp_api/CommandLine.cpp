#include "stdafx.h"
#include "CommandLine.h"


namespace plugin_cpp_api
{
   CCommandLine::CCommandLine(int argc, char **argv)
   {
      static const auto NbExpectedArguments = 2;

      if (argc != NbExpectedArguments)
         throw std::invalid_argument((boost::format("Invalid arguments number (%1%), %2% expected") % argc % NbExpectedArguments).str());

      m_yPluginApiAccessorId = argv[1];
   }

   CCommandLine::~CCommandLine()
   {
   }

   std::string CCommandLine::yPluginApiAccessorId() const
   {
      return m_yPluginApiAccessorId;
   }

} // namespace plugin_cpp_api
