#pragma once
#include <shared/script/IScriptRunner.h>
#include <shared/shared/DataContainer.h>

namespace automation { namespace action
{
   //-----------------------------------------------------
   ///\brief The script runner factory interface
   //-----------------------------------------------------
   class IScriptRunnerFactory
   {
   public:
      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~IScriptRunnerFactory() {}

      //-----------------------------------------------------
      ///\brief               Create a script runner
      ///\param[in] scriptPath    Script path
      ///\param[in] configuration Script configuration
      ///\return              The script runner instance
      //-----------------------------------------------------
      virtual boost::shared_ptr<shared::script::IScriptRunner> createScriptRunner(const std::string& scriptPath, const shared::CDataContainer& scriptConfiguration) const = 0;
   };
	
} } // namespace automation::action	
	
	