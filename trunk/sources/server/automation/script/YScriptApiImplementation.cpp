#include "stdafx.h"
#include "YScriptApiImplementation.h"
#include "../pluginSystem/DeviceCommand.h"

#include "notification/action/SelfEventAction.hpp"
#include "notification/acquisition/Observer.hpp"
#include "notification/NotificationCenter.h"
#include <shared/ServiceLocator.h>

#include <shared/plugin/yPluginApi/historization/MessageFormatter.h>
#include <shared/Log.h>
#include <shared/exception/EmptyResult.hpp>

namespace automation { namespace script
{

CYScriptApiImplementation::CYScriptApiImplementation(
   boost::shared_ptr<ILogger> ruleLogger,
   boost::shared_ptr<communication::ISendMessageAsync> pluginGateway,
   boost::shared_ptr<dataAccessLayer::IConfigurationManager> configurationManager,
   boost::shared_ptr<database::IAcquisitionRequester> dbAcquisitionRequester,
   boost::shared_ptr<IGeneralInfo> generalInfo)
   :m_ruleLogger(ruleLogger),
   m_pluginGateway(pluginGateway),
   m_dbAcquisitionRequester(dbAcquisitionRequester),
   m_generalInfo(generalInfo)
{
}

CYScriptApiImplementation::~CYScriptApiImplementation()
{
}

std::string CYScriptApiImplementation::readKeyword(int keywordId) const
{
   try
   {
      return m_dbAcquisitionRequester->getKeywordLastData(keywordId)->Value;
   }
   catch(shared::exception::CEmptyResult& e)
   {
      YADOMS_LOG(warning) << "readKeyword, ID#" << keywordId << " not found : " << e.what();
      return std::string();      
   }
   catch(...) // Must catch all exceptions to not crash script interpreter
   {
      YADOMS_LOG(error) << "readKeyword, unknown exception, please report to Yadoms team";
      return std::string();      
   }
}

std::string CYScriptApiImplementation::waitForEvent(int keywordId, const std::string& timeout) const
{
   try
   {

      boost::shared_ptr<notification::CNotificationCenter> nc = shared::CServiceLocator::instance().get<notification::CNotificationCenter>();
      if (nc)
      {
         //create the action (= what to do when notification is observed)
         boost::shared_ptr< notification::action::CSelfEventAction<notification::acquisition::CNotification> > selfAction(new notification::action::CSelfEventAction<notification::acquisition::CNotification>());
         
         //create the acquisition observer
         boost::shared_ptr<notification::acquisition::CObserver > observer(new notification::acquisition::CObserver(selfAction));

         //configure the keyword id filter
         observer->addKeywordIdFilter(keywordId);

         //regsiter the observer
         nc->subscribeObserver(observer);

         //wait for acquisition notification
         boost::shared_ptr<notification::acquisition::CNotification> newAcquisition = selfAction->wait(timeout.empty() ? boost::date_time::pos_infin : boost::posix_time::duration_from_string(timeout));

         //unsubscribe the observer
         nc->unsubscribeObserver(observer);

         if (!newAcquisition)
            return std::string();

         return newAcquisition->getAcquisition()->Value;
      }
   }
   catch(...) // Must catch all exceptions to not crash script interpreter
   {
      YADOMS_LOG(error) << "waitForEvent, unknown exception, please report to Yadoms team";
   }
   return std::string();
}

std::pair<int, std::string> CYScriptApiImplementation::waitForEvents(std::vector<int> keywordIdList, const std::string& timeout) const
{
   try
   {
      //TODO
      return std::pair<int, std::string>();
   }
   catch(...) // Must catch all exceptions to not crash script interpreter
   {
      YADOMS_LOG(error) << "waitForEvents, unknown exception, please report to Yadoms team";
      return std::pair<int, std::string>();
   }
}

void CYScriptApiImplementation::writeKeyword(int keywordId, const std::string& newState)
{
   try
   {
      m_pluginGateway->sendCommandAsync(keywordId, newState);
   }
   catch (shared::exception::CEmptyResult& e)
   {
      YADOMS_LOG(error) << "Unable to write keyword : " << e.what();
   }
   catch(...) // Must catch all exceptions to not crash script interpreter
   {
      YADOMS_LOG(error) << "writeKeyword, unknown exception, please report to Yadoms team";
   }
}

void CYScriptApiImplementation::sendNotification(int keywordId, int recipientId, const std::string& message)
{
   try
   {
      shared::plugin::yPluginApi::historization::CMessageFormatter body(0, recipientId, message);
      m_pluginGateway->sendCommandAsync(keywordId, body.formatValue());
   }
   catch(...) // Must catch all exceptions to not crash script interpreter
   {
      YADOMS_LOG(error) << "sendNotification, unknown exception, please report to Yadoms team";
   }
}

std::string CYScriptApiImplementation::getInfo(const std::string& key) const
{
   try
   {
      return m_generalInfo->get(key);
   }
   catch(...) // Must catch all exceptions to not crash script interpreter
   {
      YADOMS_LOG(error) << "getInfo, unknown exception, please report to Yadoms team";
      return std::string();
   }
}

void CYScriptApiImplementation::log(const std::string& message)
   {
   try
   {
      m_ruleLogger->logInformation(message);
      YADOMS_LOG(information) << message;
   }
   catch(...) // Must catch all exceptions to not crash script interpreter
   {
      YADOMS_LOG(error) << "log, unknown exception, please report to Yadoms team";
   }
}

void CYScriptApiImplementation::logError(const std::string& message)
   {
   try
   {
      m_ruleLogger->logError(message);
      YADOMS_LOG(error) << message;
   }
   catch(...) // Must catch all exceptions to not crash script interpreter
   {
      YADOMS_LOG(error) << "log, unknown exception, please report to Yadoms team";
   }
}

} } // namespace automation::script
	
	