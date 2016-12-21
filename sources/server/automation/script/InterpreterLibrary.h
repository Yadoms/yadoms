#pragma once
#include "IInterpreterLibrary.h"
#include <shared/DynamicLibrary.h>

namespace automation
{
   namespace script
   {
      //-----------------------------------------------------
      ///\brief The script interpreter library
      //-----------------------------------------------------
      class CInterpreterLibrary : public IInterpreterLibrary
      {
      public:
         //-----------------------------------------------------
         ///\brief               Constructor
         ///\param[in] interpreterPath Interpreter path
         ///\throw CInvalidParameter if unable to load interpreter
         //-----------------------------------------------------
         explicit CInterpreterLibrary(const boost::filesystem::path& interpreterPath);

         //-----------------------------------------------------
         ///\brief               Destructor
         //-----------------------------------------------------
         virtual ~CInterpreterLibrary();

      protected:
         // IInterpreterLibrary Implementation
         boost::shared_ptr<shared::script::IInterpreter> getInterpreter() const override;
         // [END] IInterpreterLibrary Implementation

         //-----------------------------------------------------
         ///\brief               Load the library and create interpreter
         //-----------------------------------------------------
         boost::shared_ptr<shared::script::IInterpreter> load(const boost::filesystem::path& interpreterPath);

      private:
         //-----------------------------------------------------
         ///\brief               The loaded library
         //-----------------------------------------------------
         boost::shared_ptr<shared::CDynamicLibrary> m_library;

         //-----------------------------------------------------
         ///\brief               The created interpreter
         //-----------------------------------------------------
         boost::shared_ptr<shared::script::IInterpreter> m_interpreter;
      };
   }
} // namespace automation::script


