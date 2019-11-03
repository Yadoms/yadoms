#pragma once
#include "NotificationProperties.h"
#include "UrlManagerHelper.h"


class CLametricNotificationSender
{
public:

   CLametricNotificationSender(CLametricConfiguration& lametricConfiguration);

   virtual ~CLametricNotificationSender() = default;

   void displayText(const std::string& text,
                    notificationProperties::CNotificationPriority::EPriorityType priorityType = notificationProperties::
                       CNotificationPriority::EPriorityType::kCritical,
                    notificationProperties::CNotificationIcon::EIconType iconType = notificationProperties::
                       CNotificationIcon::EIconType::kAlert);

private:

   CLametricConfiguration m_lametricConfiguration;

   boost::shared_ptr<CUrlManagerHelper> m_urlManagerHelper;
};
