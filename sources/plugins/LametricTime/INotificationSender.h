#pragma once
#include <string>
#include "NotificationProperties.h"

class INotificationSender
{
public:
   // ----------------------------------------
   /// \brief	        Destructor
   // ---------------------------------------- 
   virtual ~INotificationSender() = default;
   /**
    * \brief     Sends new notification to device
    * \param[in] text                              Text to send
    * \param[in] priorityType                      Priority of the message, optional. Valid values are info, warning, critical
    * \param[in] iconType                          Represents the nature of notification, optional. Valid values: none, info, alert
   */
   virtual void displayText(const std::string& text,
                            notificationProperties::CNotificationPriority::EPriorityType priorityType =
                               notificationProperties::
                               CNotificationPriority::EPriorityType::kCritical,
                            notificationProperties::CNotificationIcon::EIconType iconType = notificationProperties::
                               CNotificationIcon::EIconType::kAlert) = 0;
};
