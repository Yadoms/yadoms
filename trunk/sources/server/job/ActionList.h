#pragma once
#include "IActionList.h"
#include <shared/shared/DataContainer.h>
#include <server/communication/ISendMessageAsync.h>
#include <server/job/IAction.h>

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

   private:
      //-----------------------------------------------------
      ///\brief               The actions to do
      //-----------------------------------------------------
      std::vector<boost::shared_ptr<IAction> > m_actions;
   };
	
} // namespace job	
	
	