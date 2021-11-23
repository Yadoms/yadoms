#pragma once
#include "IRestService.h"
#include "database/IDataProvider.h"
#include "communication/ISendMessageAsync.h"
#include "pluginSystem/Manager.h"

namespace web
{
   namespace rest
   {
      namespace service
      {
         class CDevice final : public IRestService
         {
         public:
            CDevice(const boost::shared_ptr<database::IDataProvider>& dataProvider,
                    boost::shared_ptr<pluginSystem::CManager> pluginManager,
                    boost::shared_ptr<dataAccessLayer::IDeviceManager> deviceManager,
                    boost::shared_ptr<dataAccessLayer::IKeywordManager> keywordManager,
                    communication::ISendMessageAsync& messageSender);
            ~CDevice() override = default;

            // IRestService implementation
            void configurePocoDispatcher(poco::CRestDispatcher& dispatcher) override;
            boost::shared_ptr<std::vector<boost::shared_ptr<IRestEndPoint>>> endPoints() override;
            // [END] IRestService implementation

         private:
            //-----------------------------------------
            ///\brief   get a device
            //-----------------------------------------
            boost::shared_ptr<shared::serialization::IDataSerializable> getOneDeviceV1(
               const std::vector<std::string>& parameters,
               const std::string& requestContent) const;

            //-----------------------------------------
            ///\brief   get device compatible for merge operation
            //-----------------------------------------
            boost::shared_ptr<shared::serialization::IDataSerializable> getCompatibleForMergeDeviceV1(
               const std::vector<std::string>& parameters,
               const std::string& requestContent) const;

            //-----------------------------------------
            ///\brief   get configuration schema of specified device
            //-----------------------------------------
            boost::shared_ptr<shared::serialization::IDataSerializable> getDeviceConfigurationSchemaV1(
               const std::vector<std::string>& parameters,
               const std::string& requestContent) const;

            //-----------------------------------------
            ///\brief   get all devices
            //-----------------------------------------
            boost::shared_ptr<shared::serialization::IDataSerializable> getAllDevicesV1(
               const std::vector<std::string>& parameters,
               const std::string& requestContent) const;

            //-----------------------------------------
            ///\brief   get all devices which supports a capacity
            //-----------------------------------------
            boost::shared_ptr<shared::serialization::IDataSerializable> getDevicesWithCapacityV1(
               const std::vector<std::string>& parameters,
               const std::string& requestContent) const;

            //-----------------------------------------
            ///\brief   get all the keywords which implement a device capacity
            //-----------------------------------------
            boost::shared_ptr<shared::serialization::IDataSerializable> getDeviceKeywordsForCapacityV1(
               const std::vector<std::string>& parameters,
               const std::string& requestContent) const;

            //-----------------------------------------
            ///\brief   get all the keywords which match a capacity type
            //-----------------------------------------
            boost::shared_ptr<shared::serialization::IDataSerializable> getDeviceWithCapacityTypeV1(
               const std::vector<std::string>& parameters,
               const std::string& requestContent) const;

            //-----------------------------------------
            ///\brief   get all the devices with any keyword matching type
            //-----------------------------------------
            boost::shared_ptr<shared::serialization::IDataSerializable> getDeviceWithKeywordAccessModeV1(
               const std::vector<std::string>& parameters,
               const std::string& requestContent) const;

            //-----------------------------------------
            ///\brief   get all the devices with any keyword history depth
            //-----------------------------------------
            boost::shared_ptr<shared::serialization::IDataSerializable> getDeviceWithKeywordHistoryDepthV1(
               const std::vector<std::string>& parameters,
               const std::string& requestContent) const;

            //-----------------------------------------
            ///\brief   get all the devices matching some criteria
            //-----------------------------------------
            boost::shared_ptr<shared::serialization::IDataSerializable> getDeviceMatchKeywordCriteriaV1(
               const std::vector<std::string>& parameters,
               const std::string& requestContent) const;

            //-----------------------------------------
            ///\brief   get all the keywords of a device
            //-----------------------------------------
            boost::shared_ptr<shared::serialization::IDataSerializable> getDeviceKeywordsV1(
               const std::vector<std::string>& parameters,
               const std::string& requestContent) const;

            //-----------------------------------------
            ///\brief   get the keywords last state (from unique keyword or keyword list)
            //-----------------------------------------
            boost::shared_ptr<shared::serialization::IDataSerializable> getKeywordsLastStateV1(
               const std::vector<std::string>& parameters,
               const std::string& requestContent) const;

            //-----------------------------------------
            ///\brief   get a keyword
            //-----------------------------------------
            boost::shared_ptr<shared::serialization::IDataSerializable> getKeywordV1(
               const std::vector<std::string>& parameters,
               const std::string& requestContent) const;

            //-----------------------------------------
            ///\brief   get all keywords
            //-----------------------------------------
            boost::shared_ptr<shared::serialization::IDataSerializable> getAllKeywordsV1(
               const std::vector<std::string>& parameters,
               const std::string& requestContent) const;

            //-----------------------------------------
            ///\brief   send a command to a keyword of a device
            //-----------------------------------------
            boost::shared_ptr<shared::serialization::IDataSerializable> sendKeywordCommandV1(
               const std::vector<std::string>& parameters,
               const std::string& requestContent) const;

            //-----------------------------------------
            ///\brief   Delete a device in database
            //-----------------------------------------
            boost::shared_ptr<shared::serialization::IDataSerializable> deleteDeviceV1(
               const std::vector<std::string>& parameters,
               const std::string& requestContent) const;

            //-----------------------------------------
            ///\brief   update a device friendly name
            //-----------------------------------------
            boost::shared_ptr<shared::serialization::IDataSerializable> updateDeviceFriendlyName(
               const std::vector<std::string>& parameters,
               const std::string& requestContent) const;

            //-----------------------------------------
            ///\brief   update a device configuration
            //-----------------------------------------
            boost::shared_ptr<shared::serialization::IDataSerializable> updateDeviceConfigurationV1(
               const std::vector<std::string>& parameters,
               const std::string& requestContent) const;

            //-----------------------------------------
            ///\brief   merge 2 devices
            //-----------------------------------------
            boost::shared_ptr<shared::serialization::IDataSerializable> mergeDevicesV1(
               const std::vector<std::string>& parameters,
               const std::string& requestContent) const;

            //-----------------------------------------
            ///\brief   update a keyword friendly name
            //-----------------------------------------
            boost::shared_ptr<shared::serialization::IDataSerializable> updateKeywordFriendlyNameV1(
               const std::vector<std::string>& parameters,
               const std::string& requestContent) const;

            //-----------------------------------------
            ///\brief  Restore a device
            //-----------------------------------------
            boost::shared_ptr<shared::serialization::IDataSerializable> restoreDeviceV1(
               const std::vector<std::string>& parameters,
               const std::string& requestContent) const;

            //-----------------------------------------
            ///\brief  (un)blacklist a keyword
            //-----------------------------------------
            boost::shared_ptr<shared::serialization::IDataSerializable> updateKeywordBlacklistV1(
               const std::vector<std::string>& parameters,
               const std::string& requestContent) const;


            //-----------------------------------------
            ///\brief   Transactional method used to encapsulate operation in transactions
            //-----------------------------------------
            boost::shared_ptr<shared::serialization::IDataSerializable> transactionalMethodV1(
               const poco::CRestDispatcher::CRestMethodHandler realMethod,
               const std::vector<std::string>& parameters,
               const std::string& requestContent) const;

            boost::shared_ptr<IAnswer> getDevicesV2(const boost::shared_ptr<IRequest>& request) const;
            boost::shared_ptr<IAnswer> getDeviceDynamicConfigurationSchemaV2(const boost::shared_ptr<IRequest>& request) const;
            boost::shared_ptr<IAnswer> sendExtraQueryToDeviceV2(const boost::shared_ptr<IRequest>& request) const;
            std::string generateUniqueDeviceName(int pluginId) const;
            boost::shared_ptr<IAnswer> createDeviceV2(const boost::shared_ptr<IRequest>& request) const;
            boost::shared_ptr<IAnswer> updateDeviceV2(const boost::shared_ptr<IRequest>& request) const;
            boost::shared_ptr<IAnswer> deleteDeviceV2(const boost::shared_ptr<IRequest>& request) const;
            boost::shared_ptr<IAnswer> getKeywordsV2(const boost::shared_ptr<IRequest>& request) const;
            boost::shared_ptr<IAnswer> updateKeywordV2(const boost::shared_ptr<IRequest>& request) const;
            boost::shared_ptr<IAnswer> sendCommandV2(const boost::shared_ptr<IRequest>& request) const;

            //-----------------------------------------
            ///\brief   Data provider
            //-----------------------------------------
            boost::shared_ptr<database::IDataProvider> m_dataProvider;
            boost::shared_ptr<database::IDeviceRequester> m_deviceRequester;

            //-----------------------------------------
            ///\brief   Plugin manager (required for some operations)
            //-----------------------------------------
            boost::shared_ptr<pluginSystem::CManager> m_pluginManager;

            //-----------------------------------------
            ///\brief   Device manager
            //-----------------------------------------
            boost::shared_ptr<dataAccessLayer::IDeviceManager> m_deviceManager;

            //-----------------------------------------
            ///\brief   Keyword manager
            //-----------------------------------------
            boost::shared_ptr<dataAccessLayer::IKeywordManager> m_keywordManager;
            boost::shared_ptr<std::vector<boost::shared_ptr<IRestEndPoint>>> m_endPoints;

            //-----------------------------------------
            ///\brief   The rest keyword which identifies this rule
            //-----------------------------------------
            static std::string m_restKeyword;

            //-----------------------------------------
            ///\brief   The send message interface
            //-----------------------------------------
            communication::ISendMessageAsync& m_messageSender;
         };
      } //namespace service
   } //namespace rest
} //namespace web 
