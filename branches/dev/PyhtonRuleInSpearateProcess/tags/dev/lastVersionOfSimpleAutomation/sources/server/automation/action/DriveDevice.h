#pragma once
#include "IAction.h"
#include <server/communication/ISendMessageAsync.h>

namespace automation { namespace action
{
   //-----------------------------------------------------
   ///\brief A action for changing state of a device
   //-----------------------------------------------------
   class CDeviceDrive : public IAction
   {
   public:
      //-----------------------------------------------------
      ///\brief               Constructor
      ///\param[in] configuration Action configuration
      ///\param[in] pluginGateway Plugin gateway
      //-----------------------------------------------------
      CDeviceDrive(const shared::CDataContainer& configuration, boost::shared_ptr<communication::ISendMessageAsync> pluginGateway);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CDeviceDrive();

   protected:
      // IAction Implementation
      virtual void doAction();
      virtual void stopAction();
      // [END] IAction Implementation

   private:
      //-----------------------------------------------------
      ///\brief               The plugin gateway
      //-----------------------------------------------------
      boost::shared_ptr<communication::ISendMessageAsync> m_pluginGateway;

      //-----------------------------------------------------
      ///\brief               Keyword ID
      //-----------------------------------------------------
      const int m_keywordId;

      //-----------------------------------------------------
      ///\brief               Value to set to device
      //-----------------------------------------------------
      std::string m_valueToSet;
   };
	
} } // namespace automation::action
	
	