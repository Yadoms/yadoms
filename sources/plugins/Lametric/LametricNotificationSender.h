#pragma once
#include "NotificationProperties.h"
#include "UrlManagerHelper.h"


class CLametricNotificationSender
{
public:
   // ----------------------------------------
   /// \brief	        Constructor
   // ---------------------------------------- 
   CLametricNotificationSender(CLametricConfiguration& lametricConfiguration);
   // ----------------------------------------
   /// \brief	        Destructor
   // ---------------------------------------- 
   virtual ~CLametricNotificationSender() = default;

   /**
    * \brief     Sends new notification to device
    * \param[in] text                              Text to send
    * \param[in] priorityType                      Priority of the message, optional. Valid values are info, warning, critical
    * \param[in] iconType                          Represents the nature of notification, optional. Valid values: none, info, alert
   */
   void displayText(const std::string& text,
                    notificationProperties::CNotificationPriority::EPriorityType priorityType = notificationProperties::
                       CNotificationPriority::EPriorityType::kCritical,
                    notificationProperties::CNotificationIcon::EIconType iconType = notificationProperties::
                       CNotificationIcon::EIconType::kAlert);

private:

   CLametricConfiguration m_lametricConfiguration;

   boost::shared_ptr<CUrlManagerHelper> m_urlManagerHelper;
};
