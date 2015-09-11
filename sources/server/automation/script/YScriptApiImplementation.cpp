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
   boost::shared_ptr<database::IRecipientRequester> dbRecipientRequester,
   boost::shared_ptr<IGeneralInfo> generalInfo)
   :m_ruleLogger(ruleLogger),
   m_pluginGateway(pluginGateway),
   m_dbAcquisitionRequester(dbAcquisitionRequester),
   m_dbDeviceRequester(dbDeviceRequester),
   m_dbKeywordRequester(dbKeywordRequester),
   m_dbRecipientRequester(dbRecipientRequester),
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

void CYScriptApiImplementation::assertExistingKeyword(int keywordId) const
{
   if (!m_dbKeywordRequester->keywordExists(keywordId))
      throw std::out_of_range((boost::format("Keyword ID#%1% not found") % keywordId).str());
}

void CYScriptApiImplementation::assertExistingRecipient(int recipientId) const
{
   if (!m_dbRecipientRequester->exists(recipientId))
      throw std::out_of_range((boost::format("Recipient ID#%1% not found") % recipientId).str());
}

int CYScriptApiImplementation::getKeywordId(const std::string& deviceName, const std::string& keywordName) const
{
   std::vector<boost::shared_ptr<database::entities::CKeyword> > keywords;
   try
   {
      std::vector<boost::shared_ptr<database::entities::CDevice> > devices = m_dbDeviceRequester->getDevicesIdFromFriendlyName(deviceName);
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
   }
   catch (shared::exception::CEmptyResult&)
   {
      throw std::out_of_range((boost::format("getKeywordId, keyword %1% for device %2% not found because device was not found") % keywordName % deviceName).str());
   }
   catch (...)
   {
      throw std::out_of_range("getKeywordId, unknown exception, please report to Yadoms team");
   }

   if (keywords.size() == 1)
      return keywords[0]->Id;
   else if (keywords.size() == 0)
      throw std::out_of_range((boost::format("getKeywordId, keyword %1% for device %2% not found") % keywordName % deviceName).str());
   else
      throw std::out_of_range((boost::format("getKeywordId, several keywords found for %1% for device %2%. Please rename keywords or device.") % keywordName % deviceName).str());
}

std::string CYScriptApiImplementation::readKeyword(int keywordId) const
{
   assertExistingKeyword(keywordId);

   try
   {
      return m_dbAcquisitionRequester->getKeywordLastData(keywordId)->Value;
   }
   catch(shared::exception::CEmptyResult&)
   {
      return std::string();
   }
   catch(...)
   {
      throw std::out_of_range("readKeyword, unknown exception, please report to Yadoms team");  
   }
}

std::string CYScriptApiImplementation::waitForAcquisition(int keywordId, const std::string& timeout) const
{
   assertExistingKeyword(keywordId);

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
         return std::string(); // timeout

      return newAcquisition->getAcquisition()->Value;
   }
   catch(...)
   {
      throw std::out_of_range("waitForEvent, unknown exception, please report to Yadoms team");
   }
}

std::pair<int, std::string> CYScriptApiImplementation::waitForAcquisitions(const std::vector<int> keywordIdList, const std::string& timeout) const
{
   for (std::vector<int>::const_iterator kwId = keywordIdList.begin(); kwId != keywordIdList.end(); ++ kwId)
      assertExistingKeyword(*kwId);

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
   catch(...)
   {
      throw std::out_of_range("waitForEvents, unknown exception, please report to Yadoms team");
   }
}

void CYScriptApiImplementation::writeKeyword(int keywordId, const std::string& newState)
{
   assertExistingKeyword(keywordId);

   try
   {
      m_pluginGateway->sendCommandAsync(keywordId, newState);
   }
   catch(...)
   {
      throw std::out_of_range("writeKeyword, unknown exception, please report to Yadoms team");
   }
}

void CYScriptApiImplementation::sendNotification(int keywordId, int recipientId, const std::string& message)
{
   assertExistingKeyword(keywordId);
   assertExistingRecipient(recipientId);

   try
   {
      shared::plugin::yPluginApi::historization::CMessageFormatter body(0, recipientId, message);
      m_pluginGateway->sendCommandAsync(keywordId, body.formatValue());
   }
   catch(...)
   {
      throw std::out_of_range("sendNotification, unknown exception, please report to Yadoms team");
   }
}

std::string CYScriptApiImplementation::getInfo(EInfoKeys key) const
{
   try
   {
      return m_generalInfo->get(key);
   }
   catch (shared::exception::COutOfRange& e)
   {
      throw std::out_of_range((boost::format("getInfo(%1%), error %2%") % key % e.what()).str());
   }
   catch (...)
   {
      throw std::out_of_range("getInfo, unknown exception, please report to Yadoms team");
   }
}


void CYScriptApiImplementation::ruleEnable(bool enable)
{
   m_ruleEnabled = enable;
}

} } // namespace automation::script
	
	