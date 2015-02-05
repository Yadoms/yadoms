#pragma once
#include "IAction.h"
#include <server/communication/ISendMessageAsync.h>
#include "script/IFactory.h"

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
      ///\param[in] scriptFactory The script factory
      //-----------------------------------------------------
      CRunScript(const shared::CDataContainer& configuration, boost::shared_ptr<script::IFactory> scriptFactory);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CRunScript();

   protected:
      // IAction Implementation
      virtual void doAction();
      virtual void stopAction();
      // [END] IAction Implementation

   private:
      //-----------------------------------------------------
      ///\brief               The script factory
      //-----------------------------------------------------
     boost::shared_ptr<script::IFactory> m_scriptFactory;

      //-----------------------------------------------------
      ///\brief               The script name
      //-----------------------------------------------------
      const std::string m_scriptName;

      //-----------------------------------------------------
      ///\brief               The script configuration
      //-----------------------------------------------------
      const shared::CDataContainer m_scriptConfiguration;

      //-----------------------------------------------------
      ///\brief               The script runner
      //-----------------------------------------------------
      boost::shared_ptr<shared::script::IRunner> m_runner;
   };
	
} } // namespace automation::action
	
	