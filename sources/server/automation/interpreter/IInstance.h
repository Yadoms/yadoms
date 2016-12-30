#pragma once
#include <shared/script/yInterpreterApi/IAvalaibleRequest.h>
#include <shared/script/yInterpreterApi/ILoadScriptContentRequest.h>
#include <shared/script/yInterpreterApi/ISaveScriptContentRequest.h>
#include <shared/script/yInterpreterApi/IStartScriptRequest.h>
#include <shared/script/yInterpreterApi/IStopScriptRequest.h>
#include <shared/script/yInterpreterApi/IInformation.h>

namespace automation
{
   namespace interpreter
   {
      //-----------------------------------------------------
      ///\brief A interpreter instance
      //-----------------------------------------------------
      class IInstance
      {
      public:
         //-----------------------------------------------------
         ///\brief               Destructor
         //-----------------------------------------------------
         virtual ~IInstance()
         {
         }

         //-----------------------------------------------------
         ///\brief               Get information about the interpreter associated with this instance
         ///\return              Interpreter information
         //-----------------------------------------------------
         virtual boost::shared_ptr<const shared::script::yInterpreterApi::IInformation> aboutInterpreter() const = 0;

         //-----------------------------------------------------
         ///\brief               Check if interpreter is fully avalaible
         ///\return              true if fully avalaible (all needed ressources are working)
         //-----------------------------------------------------
         virtual bool isAvalaible() = 0;

         //-----------------------------------------------------
         ///\brief               Load the script content
         ///\param[in] scriptPath   The script path to load
         ///\return              The script content
         //-----------------------------------------------------         
         virtual std::string loadScriptContent(const std::string& scriptPath) const = 0;

         //-----------------------------------------------------
         ///\brief               Save the script content
         ///\param[in] scriptPath      The script path where to save
         ///\param[in] scriptContent   The script content to save
         //-----------------------------------------------------    
         virtual void saveScriptContent(const std::string& scriptPath,
                                        const std::string& scriptContent) const = 0;
      };
   }
} // namespace automation::interpreter


