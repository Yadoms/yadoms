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
   std::string authorizationType = "Basic ";
   std::string priorityMessage;
   std::string iconToDisplay;

   notificationProperties::CNotificationPriority::getPriorityType(priorityType, priorityMessage);
   notificationProperties::CNotificationIcon::getIconType(iconType, iconToDisplay);

   m_urlManagerHelper = boost::make_shared<CUrlManagerHelper>(m_configuration);

   const auto requestPath = m_urlManagerHelper->getRequestPath(CUrlManagerHelper::kRequestNotifications);

   const auto url = m_urlManagerHelper->getRequestUrl(m_configuration, requestPath);
   const auto body =
      "{\n\"priority\": \"" + priorityMessage + "\",\n\"icon_type\": \"" + iconToDisplay +
      "\",\n\"model\":{\n\"frames\":[\n{\n\"text\":\"Yadoms\",\n\"icon\":\"i31581\",\n\"index\":0\n},\n{\n\"text\":\"" +
      text + "\",\n\"icon\":\"i31581\"\n}\n]\n}\n}";

   auto headerPostParameters = m_urlManagerHelper->buildCommonHeaderParameters(m_configuration);
   headerPostParameters.set("Content-Length", body.length());

   try
   {
      shared::CHttpMethods::sendPostRequest(url,
                                            headerPostParameters,
                                            shared::CDataContainer(),
                                            body,
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
