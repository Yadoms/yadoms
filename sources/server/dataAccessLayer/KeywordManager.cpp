#include "stdafx.h"
#include "KeywordManager.h"
#include <shared/exception/EmptyResult.hpp>
#include <Poco/Exception.h>
#include <Poco/URI.h>
#include "notification/Helpers.hpp"

namespace dataAccessLayer
{
   CKeywordManager::CKeywordManager(boost::shared_ptr<database::IDataProvider> dataProvider)
      : m_dataProvider(dataProvider),
        m_keywordRequester(dataProvider->getKeywordRequester())
   {
   }

   CKeywordManager::~CKeywordManager()
   {
   }

   bool CKeywordManager::keywordExists(int deviceId, const std::string& keywordName) const
   {
      try
      {
         getKeyword(deviceId, keywordName);
      }
      catch (shared::exception::CEmptyResult&)
      {
         return false;
      }
      return true;
   }

   bool CKeywordManager::keywordExists(int keywordId) const
   {
      try
      {
         getKeyword(keywordId);
      }
      catch (shared::exception::CEmptyResult&)
      {
         return false;
      }
      return true;
   }

   boost::shared_ptr<database::entities::CKeyword> CKeywordManager::getKeyword(int deviceId, const std::string& keyword) const
   {
      return m_keywordRequester->getKeyword(deviceId, keyword);
   }

   boost::shared_ptr<database::entities::CKeyword> CKeywordManager::getKeyword(int keywordId) const
   {
      return m_keywordRequester->getKeyword(keywordId);
   }

   std::vector<boost::shared_ptr<database::entities::CKeyword>> CKeywordManager::getKeywordIdFromFriendlyName(int deviceId, const std::string& friendlyName) const
   {
      return m_keywordRequester->getKeywordIdFromFriendlyName(deviceId, friendlyName);
   }

   std::vector<boost::shared_ptr<database::entities::CKeyword>> CKeywordManager::getAllKeywords() const
   {
      return m_keywordRequester->getAllKeywords();
   }

   std::vector<boost::shared_ptr<database::entities::CKeyword>> CKeywordManager::getKeywords(int deviceId) const
   {
      return m_keywordRequester->getKeywords(deviceId);
   }

   std::vector<boost::shared_ptr<database::entities::CKeyword>> CKeywordManager::getKeywordsMatchingCapacity(const std::string& capacity) const
   {
      return m_keywordRequester->getKeywordsMatchingCapacity(capacity);
   }

   std::vector<boost::shared_ptr<database::entities::CKeyword>> CKeywordManager::getDeviceKeywordsWithCapacity(int deviceId, const std::string& capacityName, const shared::plugin::yPluginApi::EKeywordAccessMode& capacityAccessMode) const
   {
      return m_keywordRequester->getDeviceKeywordsWithCapacity(deviceId, capacityName, capacityAccessMode);
   }

   boost::shared_ptr<database::entities::CAcquisition> CKeywordManager::getKeywordLastAcquisition(const int keywordId, bool throwIfNotExists)
   {
      return m_keywordRequester->getKeywordLastAcquisition(keywordId, throwIfNotExists);
   }

   std::string CKeywordManager::getKeywordLastData(const int keywordId, bool throwIfNotExists)
   {
      return m_keywordRequester->getKeywordLastData(keywordId, throwIfNotExists);
   }
   
   std::vector<boost::tuple<int, std::string>> CKeywordManager::getKeywordListLastData(const std::vector<int> keywordIds)
   {
      return m_keywordRequester->getKeywordListLastData(keywordIds);
   }

   void CKeywordManager::addKeyword(const database::entities::CKeyword& newKeyword) const
   {
      m_keywordRequester->addKeyword(newKeyword);
      auto keywords = m_keywordRequester->getKeywordIdFromFriendlyName(newKeyword.DeviceId,
                                                                      newKeyword.FriendlyName);

      //post notification
      for (auto keyword = keywords.begin(); keyword != keywords.end(); ++keyword)
         notification::CHelpers::postChangeNotification(keyword[0], notification::change::EChangeType::kCreate);
   }

   void CKeywordManager::addKeyword(int deviceId, const shared::plugin::yPluginApi::historization::IHistorizable& keyword, const shared::CDataContainer& details)
   {
      addKeyword(*makeKeywordEntity(deviceId, keyword, details));
   }

   void CKeywordManager::addKeywords(int deviceId, const std::vector<boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable>>& keywords)
   {
      auto transactionalEngine = m_dataProvider->getTransactionalEngine();

      try
      {
         // Create transaction if possible
         if (transactionalEngine)
            transactionalEngine->transactionBegin();

         for (auto keyword = keywords.begin(); keyword != keywords.end(); ++keyword)
            addKeyword(deviceId, **keyword);

         if (transactionalEngine)
            transactionalEngine->transactionCommit();
      }
      catch (std::exception&)
      {
         if (transactionalEngine)
            transactionalEngine->transactionRollback();
         throw; // rethrow exception, catch is just here to handle transaction
      }
   }

   void CKeywordManager::updateKeywordFriendlyName(int deviceId, const std::string& keyword, const std::string& newFriendlyName)
   {
      auto keywordToUpdate = getKeyword(deviceId, keyword);
      if (!keywordToUpdate)
         throw shared::exception::CEmptyResult("Can not find keyword");

      updateKeywordFriendlyName(keywordToUpdate->Id(),
                                newFriendlyName);
   }

   void CKeywordManager::updateKeywordFriendlyName(int keywordId, const std::string& newFriendlyName)
   {
      m_keywordRequester->updateKeywordFriendlyName(keywordId, newFriendlyName);
   }

   void CKeywordManager::updateKeywordBlacklistState(int keywordId, const bool blacklist)
   {
      auto keywordToBlacklist = getKeyword(keywordId);

      if(blacklist)
         m_dataProvider->getAcquisitionRequester()->removeKeywordData(keywordId);
      m_keywordRequester->updateKeywordBlacklistState(keywordId, blacklist);

      //post notification
      notification::CHelpers::postChangeNotification(keywordToBlacklist, notification::change::EChangeType::kDelete);
   }

   void CKeywordManager::removeKeyword(int deviceId, const std::string& keyword)
   {
      auto keywordToDelete = getKeyword(deviceId, keyword);
      if (!keywordToDelete)
         throw shared::exception::CEmptyResult("Can not find keyword");

      removeKeyword(keywordToDelete->Id());
   }

   void CKeywordManager::removeKeyword(int keywordId)
   {
     auto keywordToDelete = getKeyword(keywordId);
     m_dataProvider->getAcquisitionRequester()->removeKeywordData(keywordId);
     m_keywordRequester->removeKeyword(keywordId);

     //post notification
     notification::CHelpers::postChangeNotification(keywordToDelete, notification::change::EChangeType::kDelete);
   }

   boost::shared_ptr<database::entities::CKeyword> CKeywordManager::makeKeywordEntity(int deviceId, const shared::plugin::yPluginApi::historization::IHistorizable& keyword, const shared::CDataContainer& details)
   {
      // Validate keyword and capacity names. They must match URI pattern
      try
      {
         Poco::URI checkCapacity(keyword.getCapacity().getName());
      }
      catch (Poco::SyntaxException& ex)
      {
         throw shared::exception::COutOfRange("The capacity name does not match naming rules : " + ex.displayText());
      }

      try
      {
         Poco::URI checkKeyword(keyword.getKeyword());
      }
      catch (Poco::SyntaxException& ex)
      {
         throw shared::exception::COutOfRange("The keyword name does not match naming rules : " + ex.displayText());
      }

      auto keywordEntity(boost::make_shared<database::entities::CKeyword>());
      keywordEntity->DeviceId = deviceId;
      keywordEntity->CapacityName = keyword.getCapacity().getName();
      keywordEntity->AccessMode = keyword.getAccessMode();
      keywordEntity->Type = keyword.getCapacity().getType();
      keywordEntity->Units = keyword.getCapacity().getUnit();
      keywordEntity->Name = keyword.getKeyword();
      keywordEntity->FriendlyName = keyword.getKeyword();
      keywordEntity->Measure = keyword.getMeasureType();
      keywordEntity->TypeInfo = keyword.getTypeInfo();
      keywordEntity->Details = details;

      return keywordEntity;
   }
} //namespace dataAccessLayer