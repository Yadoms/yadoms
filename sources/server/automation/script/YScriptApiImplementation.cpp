#include "stdafx.h"
#include "YScriptApiImplementation.h"
#include "../pluginSystem/DeviceCommand.h"

#include "notification/acquisition/Observer.hpp"
#include "notification/Helpers.hpp"

#include <shared/plugin/yPluginApi/historization/MessageFormatter.h>
#include <shared/Log.h>
#include <shared/exception/EmptyResult.hpp>

#include <shared/dateTime/DateTimeContainer.h>

namespace automation { namespace script
{

CYScriptApiImplementation::CYScriptApiImplementation(
   boost::shared_ptr<shared::process::ILogger> ruleLogger,
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

   if (keywords.size() == 1)
      return keywords[0]->Id;
   if (keywords.size() == 0)
      throw std::out_of_range((boost::format("getKeywordId, keyword %1% for device %2% not found") % keywordName % deviceName).str());
   throw std::out_of_range((boost::format("getKeywordId, several keywords found for %1% for device %2%. Please rename keywords or device.") % keywordName % deviceName).str());
}

int CYScriptApiImplementation::getRecipientId(const std::string& firstName, const std::string& lastName) const
{
   boost::shared_ptr<database::entities::CRecipient> recipients;
   try
   {
      recipients = m_dbRecipientRequester->getRecipient(firstName, lastName);
   }
   catch (shared::exception::CEmptyResult&)
   {
      throw std::out_of_range((boost::format("getRecipientId, recipient for %1% %2% not found") % firstName % lastName).str());
   }

   return recipients->Id;
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
}

boost::shared_ptr<notification::acquisition::CNotification> CYScriptApiImplementation::waitForAction(
   boost::shared_ptr<notification::action::CWaitAction<notification::acquisition::CNotification> > action,
   const std::string& timeout) const
{
   boost::shared_ptr<notification::acquisition::CNotification> newAcquisition;

   if (timeout.empty())    // No timeout
      return action->wait();
      
   // With timeout      
   return action->wait(boost::posix_time::duration_from_string(timeout));
}

std::string CYScriptApiImplementation::waitForNextAcquisition(int keywordId, const std::string& timeout) const
{
   assertExistingKeyword(keywordId);

   //create the action (= what to do when notification is observed)
   boost::shared_ptr<notification::action::CWaitAction<notification::acquisition::CNotification> > waitAction(boost::make_shared<notification::action::CWaitAction<notification::acquisition::CNotification> >());
         
   //create the acquisition observer
   boost::shared_ptr<notification::acquisition::CObserver> observer(boost::make_shared<notification::acquisition::CObserver>(waitAction));
   observer->addKeywordIdFilter(keywordId);

   //register the observer
   notification::CHelpers::CCustomSubscriber subscriber(observer);

   //wait for acquisition notification
   try
   {
      boost::shared_ptr<notification::acquisition::CNotification> newAcquisition = waitForAction(waitAction, timeout);

      if (!newAcquisition)
         return std::string(); // Timeout

      return newAcquisition->getAcquisition()->Value;
   }
   catch (std::exception& exception)
   {
      m_ruleLogger->error(std::string("waitForNextAcquisition : ") + exception.what());
      throw;
   }
}

std::pair<int, std::string> CYScriptApiImplementation::waitForNextAcquisitions(const std::vector<int> & keywordIdList, const std::string& timeout) const
{
   for (std::vector<int>::const_iterator kwId = keywordIdList.begin(); kwId != keywordIdList.end(); ++ kwId)
      assertExistingKeyword(*kwId);

   //create the action (= what to do when notification is observed)
   boost::shared_ptr<notification::action::CWaitAction<notification::acquisition::CNotification> > waitAction(boost::make_shared<notification::action::CWaitAction<notification::acquisition::CNotification> >());
         
   //create the acquisition observer
   boost::shared_ptr<notification::acquisition::CObserver> observer(boost::make_shared<notification::acquisition::CObserver>(waitAction));
   observer->resetKeywordIdFilter(keywordIdList);

   //register the observer
   notification::CHelpers::CCustomSubscriber subscriber(observer);

   //wait for acquisition notification
   try
   {
      boost::shared_ptr<notification::acquisition::CNotification> newAcquisition = waitForAction(waitAction, timeout);

      if (!newAcquisition)
          return std::make_pair<int, std::string>(kTimeout, std::string()); // Timeout

      return std::pair<int, std::string>(newAcquisition->getAcquisition()->KeywordId, newAcquisition->getAcquisition()->Value);
   }
   catch (std::exception& exception)
   {
      m_ruleLogger->error(std::string("waitForNextAcquisitions : ") + exception.what());
      throw;
   }
}

shared::script::yScriptApi::CWaitForEventResult CYScriptApiImplementation::waitForEvent(const std::vector<int> & keywordIdList, bool receiveDateTimeEvent, const std::string& timeout) const
{

   for (std::vector<int>::const_iterator kwId = keywordIdList.begin(); kwId != keywordIdList.end(); ++kwId)
      assertExistingKeyword(*kwId);

   boost::shared_ptr<shared::event::CEventHandler> eventHandler(boost::make_shared<shared::event::CEventHandler>());
   
   enum
   {
      kKeyword = shared::event::kUserFirstId,
      kTime
   };


   //create the action (= what to do when notification is observed)
   boost::shared_ptr< notification::action::CEventPtrAction<notification::acquisition::CNotification> > keywordEventAction(new notification::action::CEventPtrAction<notification::acquisition::CNotification>(eventHandler, kKeyword));

   //create the acquisition observer
   boost::shared_ptr<notification::acquisition::CObserver> observer(boost::make_shared<notification::acquisition::CObserver>(keywordEventAction));
   observer->resetKeywordIdFilter(keywordIdList);

   //register the observer
   notification::CHelpers::CCustomSubscriber subscriber(observer);


   boost::shared_ptr< notification::IObserver > dateTimeObserver;
   if (receiveDateTimeEvent)
      dateTimeObserver = notification::CHelpers::subscribeBasicObserver< shared::dateTime::CDateTimeContainer >(eventHandler, kTime);

   //wait for event
   try
   {
      int resultCode = eventHandler->waitForEvents(timeout.empty() ? boost::posix_time::pos_infin : boost::posix_time::duration_from_string(timeout));

      shared::script::yScriptApi::CWaitForEventResult result;
      switch (resultCode)
      {
         case shared::event::kTimeout:
         {
            result.setType(shared::script::yScriptApi::CWaitForEventResult::kTimeout);
            break;
         }

         case kKeyword:
         {
            boost::shared_ptr<notification::acquisition::CNotification> newAcquisition = eventHandler->getEventData< boost::shared_ptr<notification::acquisition::CNotification> >();
            result.setType(shared::script::yScriptApi::CWaitForEventResult::kKeyword);
            if (newAcquisition)
            {
               result.setKeywordId(newAcquisition->getAcquisition()->KeywordId);
               result.setValue(newAcquisition->getAcquisition()->Value);
            }
            break;
         }

         case kTime:
         {
            YADOMS_LOG(information) << "CYScriptApiImplementation : kTime";
            boost::shared_ptr< shared::dateTime::CDateTimeContainer> timeNotif = eventHandler->getEventData< boost::shared_ptr<shared::dateTime::CDateTimeContainer> > ();


            result.setType(shared::script::yScriptApi::CWaitForEventResult::kDateTime);
            if (timeNotif)
            {
               YADOMS_LOG(information) << "CYScriptApiImplementation : kTime" << timeNotif->getBoostDateTime();
               result.setValue(boost::posix_time::to_iso_string(timeNotif->getBoostDateTime()));
            }
            break;
         }
      }

      if (dateTimeObserver)
         notification::CHelpers::unsubscribeObserver(dateTimeObserver);

      return result;
   }
   catch (std::exception& exception)
   {
      m_ruleLogger->error(std::string("waitForEvent : ") + exception.what());

      if (dateTimeObserver)
         notification::CHelpers::unsubscribeObserver(dateTimeObserver);
      throw;
   }
}


void CYScriptApiImplementation::writeKeyword(int keywordId, const std::string& newState)
{
   assertExistingKeyword(keywordId);
   m_pluginGateway->sendCommandAsync(keywordId, newState);
}

void CYScriptApiImplementation::sendNotification(int keywordId, int recipientId, const std::string& message)
{
   assertExistingKeyword(keywordId);
   assertExistingRecipient(recipientId);

   shared::plugin::yPluginApi::historization::CMessageFormatter body(0, recipientId, message);
   m_pluginGateway->sendCommandAsync(keywordId, body.formatValue());
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
}


void CYScriptApiImplementation::ruleEnable(bool enable)
{
   m_ruleEnabled = enable;
}

} } // namespace automation::script
	
	