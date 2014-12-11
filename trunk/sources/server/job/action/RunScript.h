#pragma once
#include "IAction.h"
#include <server/communication/ISendMessageAsync.h>

namespace job { namespace action
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
      ///\param[in] pluginGateway Plugin gateway
      //-----------------------------------------------------
      CRunScript(const shared::CDataContainer& configuration, boost::shared_ptr<communication::ISendMessageAsync> pluginGateway);

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
      ///\brief               The plugin gateway
      //-----------------------------------------------------
      boost::shared_ptr<communication::ISendMessageAsync> m_pluginGateway;

      //-----------------------------------------------------
      ///\brief               Script path
      //-----------------------------------------------------
      std::string m_scriptPath;
   };
	
} } // namespace job::action
	
	