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
         ///\brief               Ask Interpreter to stop
         //-----------------------------------------------------
         virtual void requestToStop() = 0;

         //-----------------------------------------------------
         ///\brief               Check if interpreter is fully avalaible
         ///\return              true if fully avalaible (all needed ressources are working)
         //-----------------------------------------------------
         virtual bool isAvailable() = 0;

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
         ///\param[in] scriptInstanceId   The script instance ID
         ///\param[in] scriptPath         The script path
         ///\param[in] yScriptApiId       The script API instance ID
         ///\param[in] scriptPath         The script log path
         //-----------------------------------------------------    
         virtual void startScript(int scriptInstanceId,
                                  const boost::filesystem::path& scriptPath,
                                  const std::string& yScriptApiId,
                                  const boost::filesystem::path& scriptLogPath) const = 0;

         //-----------------------------------------------------
         ///\brief               Stop the script
         ///\param[in] scriptInstanceId   The script instance ID to stop
         //-----------------------------------------------------    
         virtual void stopScript(int scriptInstanceId) const = 0;

         //-----------------------------------------------------
         ///\brief               Purge the script log
         ///\param[in] scriptInstanceId   The script instance ID to purge log (needed when rule is running)
         ///\param[in] scriptLogPath      The script instance ID to purge log (needed when rule is not running)
         //-----------------------------------------------------    
         virtual void purgeScriptLog(int scriptInstanceId,
                                     const boost::filesystem::path& scriptLogPath) const = 0;
      };
   }
} // namespace automation::interpreter


