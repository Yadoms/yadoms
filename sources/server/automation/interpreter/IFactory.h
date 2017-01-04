#pragma once
#include "IInstance.h"

namespace automation
{
   namespace interpreter
   {
      //--------------------------------------------------------------
      /// \brief	Interpreters factory
      //--------------------------------------------------------------
      class IFactory
      {
      public:
         virtual ~IFactory()
         {
         }

         virtual boost::shared_ptr<interpreter::IInstance> createInterpreterInstance(const std::string& interpreterFileName,
                                                                                     boost::function2<void, bool, const std::string&> onInstanceStateChangedFct,
                                                                                     boost::function2<void, int, const std::string&> onScriptStoppedFct) const = 0;

         virtual boost::filesystem::path interpreterLogFile(const std::string& interpreterFileName) const = 0;
      };
   }
} // namespace automation::interpreter
