#pragma once
#include "IAction.h"
#include <server/communication/ISendMessageAsync.h>
#include "script/IRunnerFactory.h"

namespace automation { namespace action
{
   //-----------------------------------------------------
   ///\brief A action running a script
   //-----------------------------------------------------
   class CRunScript : public IAction
   {
   public:
      //-----------------------------------------------------
      ///\brief               Constructor
      ///\param[in] configuration Action configuration
      ///\param[in] scriptRunnerFactory The script runner factory
      //-----------------------------------------------------
      CRunScript(const shared::CDataContainer& configuration, boost::shared_ptr<script::IRunnerFactory> scriptRunnerFactory);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CRunScript();

   protected:
      // IAction Implementation
      virtual void doAction() const;
      // [END] IAction Implementation

   private:
      //-----------------------------------------------------
      ///\brief               The script runner factory
      //-----------------------------------------------------
     boost::shared_ptr<script::IRunnerFactory> m_scriptRunnerFactory;

      //-----------------------------------------------------
      ///\brief               The script path
      //-----------------------------------------------------
      const std::string m_scriptPath;

      //-----------------------------------------------------
      ///\brief               The script configuration
      //-----------------------------------------------------
      const shared::CDataContainer m_scriptConfiguration;
   };
	
} } // namespace automation::action
	
	