#pragma once
#include "NotificationProperties.h"
#include "UrlManagerHelper.h"
#include "INotificationSender.h"

class CNotificationSender final : public INotificationSender
{
public:
   // ----------------------------------------
   /// \brief	        Constructor
   // ---------------------------------------- 
   explicit CNotificationSender(CConfiguration& lametricConfiguration);
   // ----------------------------------------
   /// \brief	        Destructor
   // ---------------------------------------- 
   virtual ~CNotificationSender() = default;

   // ILametricDeviceState implementation
   void displayText(const std::string& text,
                    notificationProperties::CNotificationPriority::EPriorityType priorityType = notificationProperties::
                       CNotificationPriority::EPriorityType::kCritical,
                    notificationProperties::CNotificationIcon::EIconType iconType = notificationProperties::
                       CNotificationIcon::EIconType::kAlert) override;

private:

   CConfiguration m_Configuration;

   boost::shared_ptr<CUrlManagerHelper> m_urlManagerHelper;
};
