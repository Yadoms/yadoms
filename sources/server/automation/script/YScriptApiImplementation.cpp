#include "stdafx.h"
#include "YScriptApiImplementation.h"
#include "notification/acquisition/Observer.hpp"
#include "notification/Helpers.hpp"
#include <shared/plugin/yPluginApi/historization/MessageFormatter.h>
#include <shared/Log.h>
#include <shared/exception/EmptyResult.hpp>

namespace automation
{
   namespace script
   {
      CYScriptApiImplementation::CYScriptApiImplementation(boost::shared_ptr<communication::ISendMessageAsync> pluginGateway,
                                                           boost::shared_ptr<database::IAcquisitionRequester> dbAcquisitionRequester,
                                                           boost::shared_ptr<database::IDeviceRequester> dbDeviceRequester,
                                                           boost::shared_ptr<dataAccessLayer::IKeywordManager> keywordAccessLayer,
                                                           boost::shared_ptr<database::IRecipientRequester> dbRecipientRequester,
                                                           boost::shared_ptr<IGeneralInfo> generalInfo)
         : m_pluginGateway(pluginGateway),
           m_dbAcquisitionRequester(dbAcquisitionRequester),
           m_dbDeviceRequester(dbDeviceRequester),
           m_keywordAccessLayer(keywordAccessLayer),
           m_dbRecipientRequester(dbRecipientRequester),
           m_generalInfo(generalInfo)
      {
      }

      CYScriptApiImplementation::~CYScriptApiImplementation()
      {
      }

      void CYScriptApiImplementation::assertExistingKeyword(int keywordId) const
      {
         if (!m_keywordAccessLayer->keywordExists(keywordId))
            throw std::out_of_range((boost::format("Keyword ID#%1% not found") % keywordId).str());
      }

      void CYScriptApiImplementation::assertExistingRecipient(int recipientId) const
      {
         if (!m_dbRecipientRequester->exists(recipientId))
            throw std::out_of_range((boost::format("Recipient ID#%1% not found") % recipientId).str());
      }

      int CYScriptApiImplementation::getKeywordId(const std::string& deviceName, const std::string& keywordName) const
      {
         std::vector<boost::shared_ptr<database::entities::CKeyword>> keywords;
         try
         {
            auto devices = m_dbDeviceRequester->getDevicesIdFromFriendlyName(deviceName);
            for (auto device = devices.begin(); device != devices.end(); ++device)
            {
               try
               {
                  auto deviceKeywords = m_keywordAccessLayer->getKeywordIdFromFriendlyName((*device)->Id, keywordName);
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
         return m_keywordAccessLayer->getKeywordLastData(keywordId);
      }

      boost::shared_ptr<notification::acquisition::CNotification> CYScriptApiImplementation::waitForAction(boost::shared_ptr<notification::action::CWaitAction<notification::acquisition::CNotification>> action,
                                                                                                           const std::string& timeout)
      {
         boost::shared_ptr<notification::acquisition::CNotification> newAcquisition;

         if (timeout.empty()) // No timeout
            return action->wait();

         // With timeout      
         return action->wait(boost::posix_time::duration_from_string(timeout));
      }

      std::string CYScriptApiImplementation::waitForNextAcquisition(int keywordId, const std::string& timeout) const
      {
         assertExistingKeyword(keywordId);

         //create the action (= what to do when notification is observed)
         auto waitAction(boost::make_shared<notification::action::CWaitAction<notification::acquisition::CNotification>>());

         //create the acquisition observer
         auto observer(boost::make_shared<notification::acquisition::CObserver>(waitAction));
         observer->addKeywordIdFilter(keywordId);

         //register the observer
         notification::CHelpers::CCustomSubscriber subscriber(observer);

         //wait for acquisition notification
         try
         {
            auto newAcquisition = waitForAction(waitAction, timeout);

            if (!newAcquisition)
               return std::string(); // Timeout

            return newAcquisition->getAcquisition()->Value;
         }
         catch (std::exception& exception)
         {
            YADOMS_LOG(error) << "waitForNextAcquisition : " << exception.what();
            throw;
         }
      }

      std::pair<int, std::string> CYScriptApiImplementation::waitForNextAcquisitions(const std::vector<int>& keywordIdList,
                                                                                     const std::string& timeout) const
      {
         for (const auto& kwId : keywordIdList)
            assertExistingKeyword(kwId);

         //create the action (= what to do when notification is observed)
         auto waitAction(boost::make_shared<notification::action::CWaitAction<notification::acquisition::CNotification>>());

         //create the acquisition observer
         auto observer(boost::make_shared<notification::acquisition::CObserver>(waitAction));
         observer->resetKeywordIdFilter(keywordIdList);

         //register the observer
         notification::CHelpers::CCustomSubscriber subscriber(observer);

         //wait for acquisition notification
         try
         {
            auto newAcquisition = waitForAction(waitAction, timeout);

            if (!newAcquisition)
               return std::make_pair<int, std::string>(kTimeout, std::string()); // Timeout

            return std::pair<int, std::string>(newAcquisition->getAcquisition()->KeywordId, newAcquisition->getAcquisition()->Value);
         }
         catch (std::exception& exception)
         {
            YADOMS_LOG(error) << "waitForNextAcquisitions : " << exception.what();
            throw;
         }
      }

      void CYScriptApiImplementation::waitForEventTimeoutConfiguration(bool receiveDateTimeEvent,
                                                                       const std::string& timeout,
                                                                       shared::script::yScriptApi::CWaitForEventResult& result,
                                                                       boost::posix_time::time_duration& timeoutDuration)
      {
         if (receiveDateTimeEvent)
         {
            const auto now = shared::currentTime::Provider().now();
            boost::posix_time::ptime nextMinute(now.date(),
                                                boost::posix_time::time_duration(now.time_of_day().hours(),
                                                                                 now.time_of_day().minutes() + 1,
                                                                                 0));
            const auto durationForNextMinute = nextMinute - now;

            if (!timeout.empty())
            {
               // Timeout is defined, use the shortest duration between defined timeout and duration for next minute
               if (boost::posix_time::duration_from_string(timeout) < durationForNextMinute)
               {
                  timeoutDuration = boost::posix_time::duration_from_string(timeout);
                  result.setType(shared::script::yScriptApi::CWaitForEventResult::kTimeout);
                  YADOMS_LOG(debug) << "CYScriptApiImplementation::waitForEvent with timeout event of " << timeout;
               }
               else
               {
                  timeoutDuration = durationForNextMinute;
                  result.setType(shared::script::yScriptApi::CWaitForEventResult::kDateTime);
                  YADOMS_LOG(debug) << "CYScriptApiImplementation::waitForEvent with dateTime event at " << (now + timeoutDuration);
               }
            }
            else
            {
               // Not timeout defined, use duration for next minute
               result.setType(shared::script::yScriptApi::CWaitForEventResult::kDateTime);
               timeoutDuration = durationForNextMinute;
               YADOMS_LOG(debug) << "CYScriptApiImplementation::waitForEvent with dateTime event at " << (now + timeoutDuration);
            }
         }
         else
         {
            if (!timeout.empty())
            {
               timeoutDuration = boost::posix_time::duration_from_string(timeout);
               result.setType(shared::script::yScriptApi::CWaitForEventResult::kTimeout);
               YADOMS_LOG(debug) << "CYScriptApiImplementation::waitForEvent with timeout event of " << timeout;
            }
            else
            {
               timeoutDuration = boost::posix_time::pos_infin;
               YADOMS_LOG(debug) << "CYScriptApiImplementation::waitForEvent";
            }
         }
      }

      shared::script::yScriptApi::CWaitForEventResult CYScriptApiImplementation::waitForEvent(const std::vector<int>& keywordIdList,
                                                                                              bool receiveDateTimeEvent,
                                                                                              const std::string& timeout) const
      {
         enum
            {
               kKeyword = shared::event::kUserFirstId
            };

         auto eventHandler(boost::make_shared<shared::event::CEventHandler>());

         // subscriber must leave until function exits
         // ReSharper disable once CppEntityAssignedButNoRead
         // ReSharper disable once CppJoinDeclarationAndAssignment
         boost::shared_ptr<notification::CHelpers::CCustomSubscriber> subscriber;
         if (!keywordIdList.empty())
         {
            for (const auto& kwId : keywordIdList)
               assertExistingKeyword(kwId);

            //create the action (= what to do when notification is observed)
            auto keywordEventAction(boost::make_shared<notification::action::CEventPtrAction<notification::acquisition::CNotification>>(eventHandler, kKeyword));

            //create the acquisition observer
            auto observer(boost::make_shared<notification::acquisition::CObserver>(keywordEventAction));
            observer->resetKeywordIdFilter(keywordIdList);

            //register the observer
            // ReSharper disable once CppJoinDeclarationAndAssignment
            subscriber = boost::make_shared<notification::CHelpers::CCustomSubscriber>(observer);
         }

         shared::script::yScriptApi::CWaitForEventResult result;
         boost::posix_time::time_duration timeoutDuration;
         waitForEventTimeoutConfiguration(receiveDateTimeEvent,
                                          timeout,
                                          result,
                                          timeoutDuration);

         //wait for event
         try
         {
            auto resultCode = eventHandler->waitForEvents(timeoutDuration);

            switch (resultCode)
            {
            case shared::event::kTimeout:
               {
                  YADOMS_LOG(debug) << "CYScriptApiImplementation : kTimeout";
                  // Type already set (by timeout configuration)
                  result.setValue(boost::posix_time::to_iso_string(shared::currentTime::Provider().now()));
                  break;
               }

            case kKeyword:
               {
                  YADOMS_LOG(debug) << "CYScriptApiImplementation : kKeyword";
                  auto newAcquisition = eventHandler->getEventData<boost::shared_ptr<notification::acquisition::CNotification>>();
                  result.setType(shared::script::yScriptApi::CWaitForEventResult::kKeyword);
                  if (newAcquisition)
                  {
                     YADOMS_LOG(debug) << "CYScriptApiImplementation : kKeyword #" << newAcquisition->getAcquisition()->KeywordId() << " : " << newAcquisition->getAcquisition()->Value();
                     result.setKeywordId(newAcquisition->getAcquisition()->KeywordId);
                     result.setValue(newAcquisition->getAcquisition()->Value);
                  }
                  break;
               }

            default:
               {
                  YADOMS_LOG(error) << "CYScriptApiImplementation::waitForEvent : unknown result code " << resultCode;
                  throw std::out_of_range((boost::format("CYScriptApiImplementation::waitForEvent : unknown result code %1%") % resultCode).str());
               }
            }

            return result;
         }
         catch (std::exception& exception)
         {
            YADOMS_LOG(error) << "waitForEvent : " << exception.what();
            throw;
         }
      }

      std::vector<int> CYScriptApiImplementation::getKeywordsByCapacity(const std::string& capacity) const
      {
         //get list of keywords
         auto keywordMatchingCapacity = m_keywordAccessLayer->getKeywordsMatchingCapacity(capacity);

         //extract only id
         std::vector<int> result;
         for (auto i = keywordMatchingCapacity.begin(); i != keywordMatchingCapacity.end(); ++i)
            result.push_back((*i)->Id());
         return result;
      }

      void CYScriptApiImplementation::writeKeyword(int keywordId, const std::string& newState)
      {
         assertExistingKeyword(keywordId);
         m_pluginGateway->sendKeywordCommandAsync(keywordId, newState);
      }

      void CYScriptApiImplementation::sendNotification(int keywordId, int recipientId, const std::string& message)
      {
         assertExistingKeyword(keywordId);
         assertExistingRecipient(recipientId);

         shared::plugin::yPluginApi::historization::CMessageFormatter body(0, recipientId, message);
         m_pluginGateway->sendKeywordCommandAsync(keywordId, body.formatValue());
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

      std::string CYScriptApiImplementation::getKeywordName(int keywordId) const
      {
         auto keyword = m_keywordAccessLayer->getKeyword(keywordId);
         if (keyword)
            return keyword->FriendlyName();
         throw std::out_of_range((boost::format("getKeywordName(%1%), error keyword not found") % keywordId).str());
      }

      std::string CYScriptApiImplementation::getKeywordDeviceName(int keywordId) const
      {
         auto keyword = m_keywordAccessLayer->getKeyword(keywordId);
         if (keyword)
         {
            auto device = m_dbDeviceRequester->getDevice(keyword->DeviceId());
            if (device)
               return device->FriendlyName();
            throw std::out_of_range((boost::format("getKeywordDeviceName(%1%), error device not found") % keywordId).str());
         }
         throw std::out_of_range((boost::format("getKeywordDeviceName(%1%), error keyword not found") % keywordId).str());
      }
   }
} // namespace automation::script


