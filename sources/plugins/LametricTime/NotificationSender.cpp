#include "NotificationSender.h"
#include "shared/http/HttpMethods.h"
#include "shared/Log.h"
#include "shared/http/HttpException.hpp"

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
                                                  const std::string& text)
{
	std::stringstream body;
	body << "{\n\"";
	body << "priority\": \"" + priorityMessage + "\",\n\"";
	body << "icon_type\": \"" + iconToDisplay + "\",\n\"";
	body << "model\":{\n\"";
	body << "frames\":[\n";
	body << "{\n\"";
	body << "text\":\"Yadoms\",\n\"";
	body << "icon\":\"i31581\",\n\"";
	body << "index\":0\n";
	body << "},\n";
	body << "{\n\"";
	body << "text\":\"" + text + "\",\n\"";
	body << "icon\":\"i31581\"\n";
	body << "}\n";
	body << "]\n";
	body << "}\n";
	body << "}";
	return body.str();
}
