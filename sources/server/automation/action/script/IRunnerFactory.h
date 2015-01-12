#pragma once
#include <shared/script/IRunner.h>
#include <shared/shared/DataContainer.h>

namespace automation { namespace action { namespace script
{
   //-----------------------------------------------------
   ///\brief The script runner factory interface
   //-----------------------------------------------------
   class IRunnerFactory
   {
   public:
      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~IRunnerFactory() {}

      //-----------------------------------------------------
      ///\brief               Create a script runner
      ///\param[in] scriptPath    Script path
      ///\param[in] configuration Script configuration
      ///\return              The script runner instance
      //-----------------------------------------------------
      virtual boost::shared_ptr<shared::script::IRunner> createScriptRunner(const std::string& scriptPath, const shared::CDataContainer& scriptConfiguration) = 0;
   };

} } } // namespace automation::action::script
	
	