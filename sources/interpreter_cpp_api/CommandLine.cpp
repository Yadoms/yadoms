#include "stdafx.h"
#include "CommandLine.h"


namespace interpreter_cpp_api
{
   CCommandLine::CCommandLine(int argc, char **argv)
   {
      static const auto NbExpectedArguments = 2;

      if (argc != NbExpectedArguments)
         throw std::invalid_argument((boost::format("Invalid arguments number (%1%), %2% expected") % argc % NbExpectedArguments).str());

      m_yInterpreterApiAccessorId = argv[1];
   }

   CCommandLine::~CCommandLine()
   {
   }

   std::string CCommandLine::yInterpreterApiAccessorId() const
   {
      return m_yInterpreterApiAccessorId;
   }

} // namespace interpreter_cpp_api
