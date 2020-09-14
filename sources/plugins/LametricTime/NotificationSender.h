#pragma once
#include "NotificationProperties.h"
#include "UrlManagerHelper.h"
#include "INotificationSender.h"

class CNotificationSender : public INotificationSender
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

	void setCustomizeIcon(std::string& customizeIcon) override;

private:

	CConfiguration m_configuration;

	boost::shared_ptr<CUrlManagerHelper> m_urlManagerHelper;

	std::string buildMessageBody(const std::string& priorityMessage, const std::string& iconToDisplay,
	                             const std::string& text) const;

	std::string getCustomizeIcon() const;

	const static std::string IconTypeName;

	std::string m_customizeIcon;
};
