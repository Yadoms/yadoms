#pragma once
#include "IActionList.h"
#include <shared/shared/DataContainer.h>
#include <server/communication/ISendMessageAsync.h>
#include "action/IAction.h"
#include "action/script/IFactory.h"

namespace automation
{
   //-----------------------------------------------------
   ///\brief An action list
   //-----------------------------------------------------
   class CActionList : public IActionList
   {
   public:
      //-----------------------------------------------------
      ///\brief               Constructor
      ///\param[in] configuration Actions configuration
      ///\param[in] pluginGateway Plugin gateway
      ///\param[in] scriptFactory The script factory
      //-----------------------------------------------------
      CActionList(const shared::CDataContainer& configuration, boost::shared_ptr<communication::ISendMessageAsync> pluginGateway,
         boost::shared_ptr<action::script::IFactory> scriptFactory);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CActionList();

   protected:
      // IActionList Implementation
      virtual void doAll();
      virtual void stopPending();
      // [END] IActionList Implementation
      
      //-----------------------------------------------------
      ///\brief               Create an action
      ///\param[in] configuration Action configuration
      ///\param[in] pluginGateway Plugin gateway
      ///\param[in] scriptFactory The script factory
      //-----------------------------------------------------
      void createAction(const shared::CDataContainer& configuration, boost::shared_ptr<communication::ISendMessageAsync> pluginGateway,
         boost::shared_ptr<action::script::IFactory> scriptFactory);

   private:
      //-----------------------------------------------------
      ///\brief               The actions to do
      //-----------------------------------------------------
      std::vector<boost::shared_ptr<action::IAction> > m_actions;
   };
	
} // namespace automation	
	
	