#pragma once
#include "IAction.h"
#include <server/communication/ISendMessageAsync.h>

namespace job
{
   //-----------------------------------------------------
   ///\brief A job
   //-----------------------------------------------------
   class CAction : public IAction
   {
   public:
      //-----------------------------------------------------
      ///\brief               Constructor
      ///\param[in] configuration Action configuration
      ///\param[in] pluginGateway Plugin gateway
      //-----------------------------------------------------
      CAction(const shared::CDataContainer& configuration, boost::shared_ptr<communication::ISendMessageAsync> pluginGateway);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CAction();

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
      ///\brief               Device ID
      //-----------------------------------------------------
      const int m_deviceId;

      //-----------------------------------------------------
      ///\brief               Keyword ID
      //-----------------------------------------------------
      const int m_keywordId;

      //-----------------------------------------------------
      ///\brief               Value to set to device
      //-----------------------------------------------------
      std::string m_valueToSet;
   };
	
} // namespace job	
	
	