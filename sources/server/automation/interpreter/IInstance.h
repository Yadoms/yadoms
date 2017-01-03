#pragma once
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

         //-----------------------------------------------------
         ///\brief               Start the script
         ///\param[in] scriptPath      The script path
         ///\param[in] yScriptApiId    The script API instance ID
         //-----------------------------------------------------    
         virtual std::string startScript(const std::string& scriptPath,
                                         const std::string& yScriptApiId) const = 0;

         //-----------------------------------------------------
         ///\brief               Stop the script
         ///\param[in] scriptProcessId  The script process ID to stop
         //-----------------------------------------------------    
         virtual void stopScript(const std::string& scriptProcessId) const = 0;
      };
   }
} // namespace automation::interpreter


