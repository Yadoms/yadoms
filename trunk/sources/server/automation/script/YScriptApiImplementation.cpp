#include "stdafx.h"
#include "YScriptApiImplementation.h"
#include "../pluginSystem/DeviceCommand.h"

#include "notification/action/WaitAction.hpp"
#include "notification/acquisition/Notification.hpp"
#include "notification/acquisition/Observer.hpp"
#include "notification/Helpers.hpp"

#include <shared/plugin/yPluginApi/historization/MessageFormatter.h>
#include <shared/Log.h>
#include <shared/exception/EmptyResult.hpp>

namespace automation { namespace script
{

CYScriptApiImplementation::CYScriptApiImplementation(
   boost::shared_ptr<shared::script::ILogger> ruleLogger,
   boost::shared_ptr<communication::ISendMessageAsync> pluginGateway,
   boost::shared_ptr<dataAccessLayer::IConfigurationManager> configurationManager,
   boost::shared_ptr<database::IAcquisitionRequester> dbAcquisitionRequester,
   boost::shared_ptr<database::IDeviceRequester> dbDeviceRequester,
   boost::shared_ptr<database::IKeywordRequester> dbKeywordRequester,
   boost::shared_ptr<IGeneralInfo> generalInfo)
   :m_ruleLogger(ruleLogger),
   m_pluginGateway(pluginGateway),
   m_dbAcquisitionRequester(dbAcquisitionRequester),
   m_dbDeviceRequester(dbDeviceRequester),
   m_dbKeywordRequester(dbKeywordRequester),
   m_generalInfo(generalInfo),
   m_ruleEnabled(true)
{
}

CYScriptApiImplementation::~CYScriptApiImplementation()
{
}

bool CYScriptApiImplementation::ruleEnabled() const
{
   return m_ruleEnabled;
}

int CYScriptApiImplementation::getKeywordId(const std::string& deviceName, const std::string& keywordName) const
{
   try
   {
      std::vector<boost::shared_ptr<database::entities::CDevice> > devices = m_dbDeviceRequester->getDevicesIdFromFriendlyName(deviceName);
      std::vector<boost::shared_ptr<database::entities::CKeyword> > keywords;
      for (std::vector<boost::shared_ptr<database::entities::CDevice> >::const_iterator device = devices.begin(); device != devices.end(); ++device)
      {
         try
         {
            std::vector<boost::shared_ptr<database::entities::CKeyword> > deviceKeywords = m_dbKeywordRequester->getKeywordIdFromFriendlyName((*device)->Id, keywordName);
            keywords.insert(keywords.end(), deviceKeywords.begin(), deviceKeywords.end());
         }
         catch (shared::exception::CEmptyResult&)
         {
            // Keyword was not found in this device
            continue;
         }
      }

      if (keywords.size() == 0)
      {
         YADOMS_LOG(error) << "getKeywordId, keyword " << keywordName << " for device " << deviceName << " not found";
         return kUnknownKeyword;
      }
      else if (keywords.size() == 1)
      {
         return keywords[0]->Id;
      }
      else
      {
         YADOMS_LOG(error) << "getKeywordId, several keywords found for " << keywordName << " for device " << deviceName << " not found";
         return kSeveralKeywords;
      }
   }
   catch (shared::exception::CEmptyResult&)
   {
      YADOMS_LOG(error) << "getKeywordId, keyword " << keywordName << " for device " << deviceName << " not found because device was not found";
      return kUnknownKeyword;
   }
   catch (...) // Must catch all exceptions to not crash script interpreter
   {
      YADOMS_LOG(error) << "getKeywordId, unknown exception, please report to Yadoms team";
      return kUnknownKeyword;
   }
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

std::string CYScriptApiImplementation::waitForAcquisition(int keywordId, const std::string& timeout) const
{
   try
   {
      //create the action (= what to do when notification is observed)
      boost::shared_ptr<notification::action::CWaitAction<notification::acquisition::CNotification> > waitAction(new notification::action::CWaitAction<notification::acquisition::CNotification>());
         
      //create the acquisition observer
      boost::shared_ptr<notification::acquisition::CObserver> observer(new notification::acquisition::CObserver(waitAction));
      observer->addKeywordIdFilter(keywordId);

      //regsiter the observer
      notification::CHelpers::CCustomSubscriber subscriber(observer);

      //wait for acquisition notification
      boost::shared_ptr<notification::acquisition::CNotification> newAcquisition = waitAction->wait(timeout.empty() ? boost::date_time::pos_infin : boost::posix_time::duration_from_string(timeout));

      if (!newAcquisition)
         return std::string();

      return newAcquisition->getAcquisition()->Value;
   }
   catch(...) // Must catch all exceptions to not crash script interpreter
   {
      YADOMS_LOG(error) << "waitForEvent, unknown exception, please report to Yadoms team";
   }
   return std::string();
}

std::pair<int, std::string> CYScriptApiImplementation::waitForAcquisitions(const std::vector<int> keywordIdList, const std::string& timeout) const
{
   try
   {
      //create the action (= what to do when notification is observed)
      boost::shared_ptr<notification::action::CWaitAction<notification::acquisition::CNotification> > waitAction(new notification::action::CWaitAction<notification::acquisition::CNotification>());
         
      //create the acquisition observer
      boost::shared_ptr<notification::acquisition::CObserver> observer(new notification::acquisition::CObserver(waitAction));
      observer->resetKeywordIdFilter(keywordIdList);

      //regsiter the observer
      notification::CHelpers::CCustomSubscriber subscriber(observer);

      //wait for acquisition notification
      boost::shared_ptr<notification::acquisition::CNotification> newAcquisition = waitAction->wait(timeout.empty() ? boost::date_time::pos_infin : boost::posix_time::duration_from_string(timeout));

      if (!newAcquisition)
         return std::make_pair(kTimeout, std::string());

      return std::pair<int, std::string>(newAcquisition->getAcquisition()->KeywordId, newAcquisition->getAcquisition()->Value);
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

std::string CYScriptApiImplementation::getInfo(EInfoKeys key) const
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


void CYScriptApiImplementation::ruleEnable(bool enable)
{
   m_ruleEnabled = enable;
}

} } // namespace automation::script
	
	