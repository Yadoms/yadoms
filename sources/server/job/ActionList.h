#pragma once
#include "IActionList.h"
#include <shared/shared/DataContainer.h>
#include <server/communication/ISendMessageAsync.h>
#include <server/job/action/IAction.h>

namespace job
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
      //-----------------------------------------------------
      CActionList(const shared::CDataContainer& configuration, boost::shared_ptr<communication::ISendMessageAsync> pluginGateway);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CActionList();

   protected:
      // IActionList Implementation
      virtual void doAll();
      // [END] IActionList Implementation
      
      //-----------------------------------------------------
      ///\brief               Create an action
      ///\param[in] configuration Action configuration
      ///\param[in] pluginGateway Plugin gateway
      //-----------------------------------------------------
      void createAction(const shared::CDataContainer& configuration, boost::shared_ptr<communication::ISendMessageAsync> pluginGateway);

   private:
      //-----------------------------------------------------
      ///\brief               The actions to do
      //-----------------------------------------------------
      std::vector<boost::shared_ptr<action::IAction> > m_actions;
   };
	
} // namespace job	
	
	