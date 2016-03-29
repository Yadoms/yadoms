#pragma once
#include <shared/process/IRunner.h>
#include <shared/DataContainer.h>
#include <shared/process/ILogger.h>
#include <shared/process/IStopNotifier.h>
#include <shared/script/yScriptApi/IYScriptApi.h>

namespace shared { namespace script
{
   //-----------------------------------------------------
   ///\brief The script interpreter interface
   //-----------------------------------------------------
   class IInterpreter
   {
   public:
      //-----------------------------------------------------
      ///\brief                  Destructor
      //-----------------------------------------------------
      virtual ~IInterpreter() {}

      //-----------------------------------------------------
      ///\brief                  Get interpreter type ("python", "tcl", etc...)
      //-----------------------------------------------------
      virtual std::string type() const = 0;

      //-----------------------------------------------------
      ///\brief                  Get interpreter user-friendly name ("Python", "Tcl", etc...)
      //-----------------------------------------------------
      virtual std::string name() const = 0;

      //-----------------------------------------------------
      ///\brief                  Check if the interpreter is fully functionnal
      ///\return                 true if the interpreter is fully functionnal
      ///\note This method is usefull when interpreter depends on libraries or services, that
      /// are optional on system (for example : Python interpreter). It checks that all the chain
      /// is ready to interpret a script file.
      //-----------------------------------------------------
      virtual bool isAvailable() const = 0;

      //-----------------------------------------------------
      ///\brief                  Load the specified script content. If not found (new script), returns a script template.
      ///\param[in] scriptPath   Script path (without name)
      ///\return                 The script content for editing, or template if not found
      //-----------------------------------------------------
      virtual std::string loadScriptContent(const std::string& scriptPath) const = 0;

      //-----------------------------------------------------
      ///\brief                  Save and overwrite specified script with provided content
      ///\param[in] scriptPath   Script path (without name)
      ///\param[in] content      Script content (code)
      //-----------------------------------------------------
      virtual void saveScriptContent(const std::string& scriptPath, const std::string& content) const = 0;

      //-----------------------------------------------------
      ///\brief                  Create a script runner with associated file
      ///\param[in] scriptPath            Script path (without name)
      ///\param[in] scriptLogger          The rule logger
      ///\param[in] yScriptApi            The rule context
      ///\param[in] stopNotifier          The stop notifier
      ///\param[in] scriptConfiguration   Configuration of the script
      ///\return                 The created script instance
      //-----------------------------------------------------
      virtual boost::shared_ptr<process::IRunner> createRunner(
         const std::string& scriptPath,
         boost::shared_ptr<process::ILogger> scriptLogger,
         boost::shared_ptr<yScriptApi::IYScriptApi> yScriptApi,
         boost::shared_ptr<process::IStopNotifier> stopNotifier,
         const shared::CDataContainer& scriptConfiguration) const = 0;
   };
	
} } // namespace shared::script
	
	