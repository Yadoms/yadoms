#include "NotificationSender.h"
#include "shared/http/HttpMethods.h"
#include "shared/Log.h"
#include "shared/http/HttpException.hpp"
#include "CustomizeIconHelper.h"

const std::string CNotificationSender::IconTypeName("iconType");
CNotificationSender::CNotificationSender(CConfiguration& lametricConfiguration)
	: m_configuration(lametricConfiguration)
{
}

void CNotificationSender::displayText(const std::string& text,
                                      notificationProperties::CNotificationPriority::EPriorityType priorityType,
                                      notificationProperties::CNotificationIcon::EIconType iconType)
{
	std::string priorityMessage;
	std::string iconToDisplay;

	notificationProperties::CNotificationPriority::getPriorityType(priorityType, priorityMessage);
	notificationProperties::CNotificationIcon::getIconType(iconType, iconToDisplay);

	m_urlManagerHelper = boost::make_shared<CUrlManagerHelper>(m_configuration);

	const auto requestPath = m_urlManagerHelper->getRequestPath(CUrlManagerHelper::kRequestNotifications);

	const auto url = m_urlManagerHelper->getRequestUrl(m_configuration, requestPath);

	const auto body = buildMessageBody(priorityMessage, iconToDisplay, text);


	auto headerPostParameters = m_urlManagerHelper->buildCommonHeaderParameters(m_configuration);
	headerPostParameters.set("Content-Length", body.length());

	try
	{
		shared::CHttpMethods::sendPostRequest(url,
		                                      body,
		                                      headerPostParameters,
		                                      shared::CDataContainer(),
		                                      m_configuration.getPort() == kHttp
			                                      ? shared::CHttpMethods::ESessionType::kStandard
			                                      : shared::CHttpMethods::ESessionType::kSecured);
	}
	catch (std::exception& e)
	{
		const auto message = (boost::format("Fail to send Post http request or interpret answer \"%1%\" : %2%") % url %
			e.what()).str();
		YADOMS_LOG(error) << message;
		throw shared::CHttpException(message);
	}
}

std::string CNotificationSender::buildMessageBody(const std::string& priorityMessage, const std::string& iconToDisplay,
                                                  const std::string& text) const
{
	std::string secondFrameIcon = !getCustomizeIcon().empty() ? m_customizeIcon : CCustomizeIconHelper::YadomsIcon;
	shared::CDataContainer body;
	body.set("prority", priorityMessage);
	body.set("icon_type", iconToDisplay);

	shared::CDataContainer frame1;
	frame1.set("text", "Yadoms");
	frame1.set("icon", CCustomizeIconHelper::YadomsIcon);
	frame1.set("index", 0);

	shared::CDataContainer frame2;
	frame2.set("text", text);
	frame2.set("icon", secondFrameIcon);

	std::vector<shared::CDataContainer> frames;
	frames.push_back(frame1);
	frames.push_back(frame2);

	shared::CDataContainer model;
	model.set("frames", frames);

	body.set("model", model);

	return body.serialize();
}

void CNotificationSender::setCustomizeIcon(std::string& customizeIcon)
{
	m_customizeIcon = CCustomizeIconHelper::getIconType(customizeIcon);;
}

std::string CNotificationSender::getCustomizeIcon() const
{
	return m_customizeIcon;
}
