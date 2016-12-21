#pragma once
#include <shared/script/IInterpreter.h>

namespace automation
{
   namespace script
   {
      //-----------------------------------------------------
      ///\brief The script interpreter library interface
      //-----------------------------------------------------
      class IInterpreterLibrary
      {
      public:
         //-----------------------------------------------------
         ///\brief               Destructor
         //-----------------------------------------------------
         virtual ~IInterpreterLibrary()
         {
         }

         //-----------------------------------------------------
         ///\brief               Get the loaded script interpreter
         ///\param[in] interpreterPath    Interpreter path
         ///\return              The loaded script interpreter
         //-----------------------------------------------------
         virtual boost::shared_ptr<shared::script::IInterpreter> getInterpreter() const = 0;
      };
   }
} // namespace automation::script


